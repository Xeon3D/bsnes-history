#ifdef CARTRIDGE_CPP

void Cartridge::parse_xml(const lstring &list) {
  mapping.reset();
  parse_xml_cartridge(list[0]);

  if(mode == Mode::BsxSlotted) {
    parse_xml_bsx(list[1]);
  } else if(mode == Mode::Bsx) {
    parse_xml_bsx(list[1]);
  } else if(mode == Mode::SufamiTurbo) {
    parse_xml_sufami_turbo(list[1], 0);
    parse_xml_sufami_turbo(list[2], 1);
  } else if(mode == Mode::SuperGameBoy) {
    parse_xml_gameboy(list[1]);
  }
}

void Cartridge::parse_xml_cartridge(const char *data) {
  //reset cartridge information
  information.nss.setting.reset();

  xml_element document = xml_parse(data);
  if(document.element.size() == 0) return;

  foreach(head, document.element) {
    if(head.name == "cartridge") {
      foreach(attr, head.attribute) {
        if(attr.name == "region") {
          if(attr.content == "NTSC") region = Region::NTSC;
          if(attr.content == "PAL") region = Region::PAL;
        }
      }

      foreach(node, head.element) {
        if(node.name == "rom") xml_parse_rom(node);
        if(node.name == "ram") xml_parse_ram(node);
        if(node.name == "nss") xml_parse_nss(node);
        if(node.name == "icd2") xml_parse_icd2(node);
        if(node.name == "superfx") xml_parse_superfx(node);
        if(node.name == "sa1") xml_parse_sa1(node);
        if(node.name == "necdsp") xml_parse_necdsp(node);
        if(node.name == "hitachidsp") xml_parse_hitachidsp(node);
        if(node.name == "bsx") xml_parse_bsx(node);
        if(node.name == "sufamiturbo") xml_parse_sufamiturbo(node);
        if(node.name == "srtc") xml_parse_srtc(node);
        if(node.name == "sdd1") xml_parse_sdd1(node);
        if(node.name == "spc7110") xml_parse_spc7110(node);
        if(node.name == "obc1") xml_parse_obc1(node);
        if(node.name == "setarisc") xml_parse_setarisc(node);
        if(node.name == "msu1") xml_parse_msu1(node);
        if(node.name == "serial") xml_parse_serial(node);
        if(node.name == "link") xml_parse_link(node);
      }
    }
  }
}

void Cartridge::parse_xml_bsx(const char *data) {
  has_bsx_slot = true;
}

void Cartridge::parse_xml_sufami_turbo(const char *data, bool slot) {
}

void Cartridge::parse_xml_gameboy(const char *data) {
}

void Cartridge::xml_parse_rom(xml_element &root) {
  foreach(leaf, root.element) {
    if(leaf.name == "map") {
      Mapping m(rom);
      foreach(attr, leaf.attribute) {
        if(attr.name == "address") xml_parse_address(m, attr.content);
        if(attr.name == "mode") xml_parse_mode(m, attr.content);
        if(attr.name == "offset") m.offset = hex(attr.content);
        if(attr.name == "size") m.size = hex(attr.content);
      }
      if(m.size == 0) m.size = rom.size();
      mapping.append(m);
    }
  }
}

void Cartridge::xml_parse_ram(xml_element &root) {
  foreach(attr, root.attribute) {
    if(attr.name == "size") ram_size = hex(attr.content);
  }

  foreach(leaf, root.element) {
    if(leaf.name == "map") {
      Mapping m(ram);
      foreach(attr, leaf.attribute) {
        if(attr.name == "address") xml_parse_address(m, attr.content);
        if(attr.name == "mode") xml_parse_mode(m, attr.content);
        if(attr.name == "offset") m.offset = hex(attr.content);
        if(attr.name == "size") m.size = hex(attr.content);
      }
      if(m.size == 0) m.size = ram_size;
      mapping.append(m);
    }
  }
}

void Cartridge::xml_parse_nss(xml_element &root) {
  has_nss_dip = true;

  foreach(node, root.element) {
    if(node.name == "setting") {
      unsigned number = information.nss.setting.size();
      if(number >= 16) break;  //more than 16 DIP switches is not possible

      information.nss.option[number].reset();
      foreach(attr, node.attribute) {
        if(attr.name == "name") {
          information.nss.setting[number] = attr.parse();
        }
      }
      foreach(leaf, node.element) {
        string name;
        unsigned value = 0x0000;
        foreach(attr, leaf.attribute) {
          if(attr.name == "name") name = attr.parse();
          if(attr.name == "value") value = (uint16)hex(attr.content);
        }
        information.nss.option[number].append({ hex<4>(value), ":", name });
      }
    }
  }
}

void Cartridge::xml_parse_icd2(xml_element &root) {
  if(mode != Mode::SuperGameBoy) return;
  icd2.revision = 1;

  foreach(attr, root.attribute) {
    if(attr.name == "revision") {
      if(attr.content == "1") icd2.revision = 1;
      if(attr.content == "2") icd2.revision = 2;
    }
  }

  foreach(node, root.element) {
    if(node.name == "map") {
      Mapping m({ &ICD2::read, &icd2 }, { &ICD2::write, &icd2 });
      foreach(attr, node.attribute) {
        if(attr.name == "address") xml_parse_address(m, attr.content);
      }
      mapping.append(m);
    }
  }
}

void Cartridge::xml_parse_superfx(xml_element &root) {
  has_superfx = true;

  foreach(node, root.element) {
    if(node.name == "rom") {
      foreach(leaf, node.element) {
        if(leaf.name == "map") {
          Mapping m(superfx.rom);
          foreach(attr, leaf.attribute) {
            if(attr.name == "address") xml_parse_address(m, attr.content);
            if(attr.name == "mode") xml_parse_mode(m, attr.content);
            if(attr.name == "offset") m.offset = hex(attr.content);
            if(attr.name == "size") m.size = hex(attr.content);
          }
          mapping.append(m);
        }
      }
    } else if(node.name == "ram") {
      foreach(attr, node.attribute) {
        if(attr.name == "size") ram_size = hex(attr.content);
      }

      foreach(leaf, node.element) {
        if(leaf.name == "map") {
          Mapping m(superfx.ram);
          foreach(attr, leaf.attribute) {
            if(attr.name == "address") xml_parse_address(m, attr.content);
            if(attr.name == "mode") xml_parse_mode(m, attr.content);
            if(attr.name == "offset") m.offset = hex(attr.content);
            if(attr.name == "size") m.size = hex(attr.content);
          }
          if(m.size == 0) m.size = ram_size;
          mapping.append(m);
        }
      }
    } else if(node.name == "mmio") {
      foreach(leaf, node.element) {
        if(leaf.name == "map") {
          Mapping m({ &SuperFX::mmio_read, &superfx }, { &SuperFX::mmio_write, &superfx });
          foreach(attr, leaf.attribute) {
            if(attr.name == "address") xml_parse_address(m, attr.content);
          }
          mapping.append(m);
        }
      }
    }
  }
}

void Cartridge::xml_parse_sa1(xml_element &root) {
  has_sa1 = true;

  foreach(node, root.element) {
    if(node.name == "mcu") {
      foreach(subnode, node.element) {
        if(subnode.name == "rom") {
          foreach(leaf, subnode.element) {
            if(leaf.name == "map") {
              Mapping m({ &SA1::mmc_read, &sa1 }, { &SA1::mmc_write, &sa1 });
              foreach(attr, leaf.attribute) {
                if(attr.name == "address") xml_parse_address(m, attr.content);
                if(attr.name == "mode") xml_parse_mode(m, attr.content);
                if(attr.name == "offset") m.offset = hex(attr.content);
                if(attr.name == "size") m.size = hex(attr.content);
              }
              mapping.append(m);
            }
          }
        } else if(subnode.name == "ram") {
          foreach(leaf, subnode.element) {
            if(leaf.name == "map") {
              Mapping m({ &SA1::mmc_cpu_read, &sa1 }, { &SA1::mmc_cpu_write, &sa1 });
              foreach(attr, leaf.attribute) {
                if(attr.name == "address") xml_parse_address(m, attr.content);
              }
              mapping.append(m);
            }
          }
        }
      }
    } else if(node.name == "iram") {
      foreach(leaf, node.element) {
        if(leaf.name == "map") {
          Mapping m(sa1.cpuiram);
          foreach(attr, leaf.attribute) {
            if(attr.name == "address") xml_parse_address(m, attr.content);
            if(attr.name == "mode") xml_parse_mode(m, attr.content);
            if(attr.name == "offset") m.offset = hex(attr.content);
            if(attr.name == "size") m.size = hex(attr.content);
          }
          if(m.size == 0) m.size = 2048;
          mapping.append(m);
        }
      }
    } else if(node.name == "bwram") {
      foreach(attr, node.attribute) {
        if(attr.name == "size") ram_size = hex(attr.content);
      }

      foreach(leaf, node.element) {
        if(leaf.name == "map") {
          Mapping m(sa1.cpubwram);
          foreach(attr, leaf.attribute) {
            if(attr.name == "address") xml_parse_address(m, attr.content);
            if(attr.name == "mode") xml_parse_mode(m, attr.content);
            if(attr.name == "offset") m.offset = hex(attr.content);
            if(attr.name == "size") m.size = hex(attr.content);
          }
          if(m.size == 0) m.size = ram_size;
          mapping.append(m);
        }
      }
    } else if(node.name == "mmio") {
      foreach(leaf, node.element) {
        if(leaf.name == "map") {
          Mapping m({ &SA1::mmio_read, &sa1 }, { &SA1::mmio_write, &sa1 });
          foreach(attr, leaf.attribute) {
            if(attr.name == "address") xml_parse_address(m, attr.content);
          }
          mapping.append(m);
        }
      }
    }
  }
}

void Cartridge::xml_parse_necdsp(xml_element &root) {
  has_necdsp = true;
  necdsp.revision = NECDSP::Revision::uPD7725;
  necdsp.frequency = 8000000;

  for(unsigned n = 0; n < 16384; n++) necdsp.programROM[n] = 0x000000;
  for(unsigned n = 0; n <  2048; n++) necdsp.dataROM[n] = 0x0000;

  string program, programhash;
  string sha256;

  foreach(attr, root.attribute) {
    if(attr.name == "revision") {
      if(attr.content == "upd7725" ) necdsp.revision = NECDSP::Revision::uPD7725;
      if(attr.content == "upd96050") necdsp.revision = NECDSP::Revision::uPD96050;
    } else if(attr.name == "frequency") {
      necdsp.frequency = decimal(attr.content);
    } else if(attr.name == "program") {
      program = attr.content;
    } else if(attr.name == "sha256") {
      sha256 = attr.content;
    }
  }

  string path = { dir(system.interface->path(Slot::Base, ".dsp")), program };
  unsigned promsize = (necdsp.revision == NECDSP::Revision::uPD7725 ? 2048 : 16384);
  unsigned dromsize = (necdsp.revision == NECDSP::Revision::uPD7725 ? 1024 :  2048);
  unsigned filesize = promsize * 3 + dromsize * 2;

  file fp;
  if(fp.open(path, file::mode::read)) {
    if(fp.size() == filesize) {
      for(unsigned n = 0; n < promsize; n++) necdsp.programROM[n] = fp.readm(3);
      for(unsigned n = 0; n < dromsize; n++) necdsp.dataROM[n] = fp.readm(2);

      fp.seek(0);
      uint8_t data[filesize];
      fp.read(data, filesize);

      sha256_ctx sha;
      uint8 shahash[32];
      sha256_init(&sha);
      sha256_chunk(&sha, data, filesize);
      sha256_final(&sha);
      sha256_hash(&sha, shahash);
      foreach(n, shahash) programhash.append(hex<2>(n));
    }
    fp.close();
  }

  foreach(node, root.element) {
    if(node.name == "dr") {
      foreach(leaf, node.element) {
        if(leaf.name == "map") {
          Mapping m({ &NECDSP::dr_read, &necdsp }, { &NECDSP::dr_write, &necdsp });
          foreach(attr, leaf.attribute) {
            if(attr.name == "address") xml_parse_address(m, attr.content);
          }
          mapping.append(m);
        }
      }
    } else if(node.name == "sr") {
      foreach(leaf, node.element) {
        if(leaf.name == "map") {
          Mapping m({ &NECDSP::sr_read, &necdsp }, { &NECDSP::sr_write, &necdsp });
          foreach(attr, leaf.attribute) {
            if(attr.name == "address") xml_parse_address(m, attr.content);
          }
          mapping.append(m);
        }
      }
    } else if(node.name == "dp") {
      foreach(leaf, node.element) {
        if(leaf.name == "map") {
          Mapping m({ &NECDSP::dp_read, &necdsp }, { &NECDSP::dp_write, &necdsp });
          foreach(attr, leaf.attribute) {
            if(attr.name == "address") xml_parse_address(m, attr.content);
          }
          mapping.append(m);
        }
      }
    }
  }

  if(programhash == "") {
    system.interface->message({ "Warning: NEC DSP program ", program, " is missing." });
  } else if(sha256 != "" && sha256 != programhash) {
    system.interface->message({
      "Warning: NEC DSP program ", program, " SHA256 is incorrect.\n\n"
      "Expected:\n", sha256, "\n\n"
      "Actual:\n", programhash
    });
  }
}

void Cartridge::xml_parse_hitachidsp(xml_element &root) {
  has_hitachidsp = true;
  hitachidsp.frequency = 20000000;

  for(unsigned n = 0; n < 1024; n++) hitachidsp.dataROM[n] = 0x000000;

  string program, sha256;

  foreach(attr, root.attribute) {
    if(attr.name == "frequency") {
      hitachidsp.frequency = decimal(attr.content);
    } else if(attr.name == "program") {
      program = attr.content;
    } else if(attr.name == "sha256") {
      sha256 = attr.content;
    }
  }

  string path = { dir(system.interface->path(Slot::Base, ".dsp")), program };
  file fp;
  if(fp.open(path, file::mode::read) == false) {
    system.interface->message({ "Warning: Hitachi DSP program ", program, " is missing." });
  } else if(fp.size() != 1024 * 3) {
    system.interface->message({ "Warning: Hitachi DSP program ", program, " is of the wrong file size." });
    fp.close();
  } else {
    for(unsigned n = 0; n < 1024; n++) hitachidsp.dataROM[n] = fp.readl(3);

    if(sha256 != "") {
      //XML file specified SHA256 sum for program. Verify file matches the hash.
      fp.seek(0);
      uint8 data[3072];
      fp.read(data, 3072);

      sha256_ctx sha;
      uint8 hash[32];
      sha256_init(&sha);
      sha256_chunk(&sha, data, 3072);
      sha256_final(&sha);
      sha256_hash(&sha, hash);

      string filehash;
      foreach(n, hash) filehash.append(hex<2>(n));

      if(sha256 != filehash) {
        system.interface->message({ "Warning: Hitachi DSP program ", program, " SHA256 sum is incorrect." });
      }
    }

    fp.close();
  }

  foreach(node, root.element) {
    if(node.name == "rom") foreach(leaf, node.element) {
      if(leaf.name == "map") {
        Mapping m({ &HitachiDSP::rom_read, &hitachidsp }, { &HitachiDSP::rom_write, &hitachidsp });
        foreach(attr, leaf.attribute) {
          if(attr.name == "address") xml_parse_address(m, attr.content);
          if(attr.name == "mode") xml_parse_mode(m, attr.content);
          if(attr.name == "offset") m.offset = hex(attr.content);
          if(attr.name == "size") m.size = hex(attr.content);
        }
        mapping.append(m);
      }
    }
    if(node.name == "mmio") foreach(leaf, node.element) {
      Mapping m({ &HitachiDSP::dsp_read, &hitachidsp }, { &HitachiDSP::dsp_write, &hitachidsp });
      foreach(attr, leaf.attribute) {
        if(attr.name == "address") xml_parse_address(m, attr.content);
      }
      mapping.append(m);
    }
  }
}

void Cartridge::xml_parse_bsx(xml_element &root) {
  if(mode != Mode::BsxSlotted && mode != Mode::Bsx) return;

  foreach(node, root.element) {
    if(node.name == "slot") {
      foreach(leaf, node.element) {
        if(leaf.name == "map") {
          Mapping m(bsxflash.memory);
          foreach(attr, leaf.attribute) {
            if(attr.name == "address") xml_parse_address(m, attr.content);
            if(attr.name == "mode") xml_parse_mode(m, attr.content);
            if(attr.name == "offset") m.offset = hex(attr.content);
            if(attr.name == "size") m.size = hex(attr.content);
          }
          mapping.append(m);
        }
      }
    } else if(node.name == "mcu") {
      foreach(leaf, node.element) {
        if(leaf.name == "map") {
          Mapping m({ &BSXCartridge::mcu_read, &bsxcartridge }, { &BSXCartridge::mcu_write, &bsxcartridge });
          foreach(attr, leaf.attribute) {
            if(attr.name == "address") xml_parse_address(m, attr.content);
          }
          mapping.append(m);
        }
      }
    } else if(node.name == "mmio") {
      foreach(leaf, node.element) {
        if(leaf.name == "map") {
          Mapping m({ &BSXCartridge::mmio_read, &bsxcartridge }, { &BSXCartridge::mmio_write, &bsxcartridge });
          foreach(attr, leaf.attribute) {
            if(attr.name == "address") xml_parse_address(m, attr.content);
          }
          mapping.append(m);
        }
      }
    }
  }
}

void Cartridge::xml_parse_sufamiturbo(xml_element &root) {
  if(mode != Mode::SufamiTurbo) return;

  foreach(node, root.element) {
    if(node.name == "slot") {
      bool slotid = 0;
      foreach(attr, node.attribute) {
        if(attr.name == "id") {
          if(attr.content == "A") slotid = 0;
          if(attr.content == "B") slotid = 1;
        }
      }

      foreach(slot, node.element) {
        if(slot.name == "rom") {
          foreach(leaf, slot.element) {
            if(leaf.name == "map") {
              Memory &memory = slotid == 0 ? sufamiturbo.slotA.rom : sufamiturbo.slotB.rom;
              Mapping m(memory);
              foreach(attr, leaf.attribute) {
                if(attr.name == "address") xml_parse_address(m, attr.content);
                if(attr.name == "mode") xml_parse_mode(m, attr.content);
                if(attr.name == "offset") m.offset = hex(attr.content);
                if(attr.name == "size") m.size = hex(attr.content);
              }
              if(m.size == 0) m.size = memory.size();
              if(m.size) mapping.append(m);
            }
          }
        } else if(slot.name == "ram") {
          unsigned ram_size = 0;

          foreach(attr, slot.attribute) {
            if(attr.name == "size") ram_size = hex(attr.content);
          }

          foreach(leaf, slot.element) {
            if(leaf.name == "map") {
              Memory &memory = slotid == 0 ? sufamiturbo.slotA.ram : sufamiturbo.slotB.ram;
              Mapping m(memory);
              foreach(attr, leaf.attribute) {
                if(attr.name == "address") xml_parse_address(m, attr.content);
                if(attr.name == "mode") xml_parse_mode(m, attr.content);
                if(attr.name == "offset") m.offset = hex(attr.content);
                if(attr.name == "size") m.size = hex(attr.content);
              }
              if(m.size == 0) m.size = ram_size;
              if(m.size) mapping.append(m);
            }
          }
        }
      }
    }
  }
}

void Cartridge::xml_parse_srtc(xml_element &root) {
  has_srtc = true;

  foreach(node, root.element) {
    if(node.name == "map") {
      Mapping m({ &SRTC::read, &srtc }, { &SRTC::write, &srtc });
      foreach(attr, node.attribute) {
        if(attr.name == "address") xml_parse_address(m, attr.content);
      }
      mapping.append(m);
    }
  }
}

void Cartridge::xml_parse_sdd1(xml_element &root) {
  has_sdd1 = true;

  foreach(node, root.element) {
    if(node.name == "mcu") {
      foreach(leaf, node.element) {
        if(leaf.name == "map") {
          Mapping m({ &SDD1::mcu_read, &sdd1 }, { &SDD1::mcu_write, &sdd1 });
          foreach(attr, leaf.attribute) {
            if(attr.name == "address") xml_parse_address(m, attr.content);
          }
          mapping.append(m);
        }
      }
    } else if(node.name == "mmio") {
      foreach(leaf, node.element) {
        if(leaf.name == "map") {
          Mapping m({ &SDD1::mmio_read, &sdd1 }, { &SDD1::mmio_write, &sdd1 });
          foreach(attr, leaf.attribute) {
            if(attr.name == "address") xml_parse_address(m, attr.content);
          }
          mapping.append(m);
        }
      }
    }
  }
}

void Cartridge::xml_parse_spc7110(xml_element &root) {
  has_spc7110 = true;
  spc7110.data_rom_offset = 0x100000;

  foreach(node, root.element) {
    if(node.name == "dcu") {
      foreach(leaf, node.element) {
        if(leaf.name == "map") {
          Mapping m({ &SPC7110::dcu_read, &spc7110 }, { &SPC7110::dcu_write, &spc7110 });
          foreach(attr, leaf.attribute) {
            if(attr.name == "address") xml_parse_address(m, attr.content);
          }
          mapping.append(m);
        }
      }
    } else if(node.name == "mcu") {
      foreach(leaf, node.element) {
        if(leaf.name == "map") {
          Mapping m({ &SPC7110::mcu_read, &spc7110 }, { &SPC7110::mcu_write, &spc7110 });
          foreach(attr, leaf.attribute) {
            if(attr.name == "address") xml_parse_address(m, attr.content);
            if(attr.name == "offset") spc7110.data_rom_offset = hex(attr.content);
          }
          mapping.append(m);
        }
      }
    } else if(node.name == "mmio") {
      foreach(leaf, node.element) {
        if(leaf.name == "map") {
          Mapping m({ &SPC7110::mmio_read, &spc7110 }, { &SPC7110::mmio_write, &spc7110 });
          foreach(attr, leaf.attribute) {
            if(attr.name == "address") xml_parse_address(m, attr.content);
          }
          mapping.append(m);
        }
      }
    } else if(node.name == "ram") {
      foreach(attr, node.attribute) {
        if(attr.name == "size") ram_size = hex(attr.content);
      }

      foreach(leaf, node.element) {
        if(leaf.name == "map") {
          Mapping m({ &SPC7110::ram_read, &spc7110 }, { &SPC7110::ram_write, &spc7110 });
          foreach(attr, leaf.attribute) {
            if(attr.name == "address") xml_parse_address(m, attr.content);
            if(attr.name == "mode") xml_parse_mode(m, attr.content);
            if(attr.name == "offset") m.offset = hex(attr.content);
            if(attr.name == "size") m.size = hex(attr.content);
          }
          mapping.append(m);
        }
      }
    } else if(node.name == "rtc") {
      has_spc7110rtc = true;

      foreach(leaf, node.element) {
        if(leaf.name == "map") {
          Mapping m({ &SPC7110::mmio_read, &spc7110 }, { &SPC7110::mmio_write, &spc7110 });
          foreach(attr, leaf.attribute) {
            if(attr.name == "address") xml_parse_address(m, attr.content);
          }
          mapping.append(m);
        }
      }
    }
  }
}

void Cartridge::xml_parse_obc1(xml_element &root) {
  has_obc1 = true;

  foreach(node, root.element) {
    if(node.name == "map") {
      Mapping m({ &OBC1::read, &obc1 }, { &OBC1::write, &obc1 });
      foreach(attr, node.attribute) {
        if(attr.name == "address") xml_parse_address(m, attr.content);
      }
      mapping.append(m);
    }
  }
}

void Cartridge::xml_parse_setarisc(xml_element &root) {
  has_st0018 = true;

  foreach(node, root.element) {
    if(node.name == "map") {
      Mapping m({ &ST0018::mmio_read, &st0018 }, { &ST0018::mmio_write, &st0018 });
      foreach(attr, node.attribute) {
        if(attr.name == "address") xml_parse_address(m, attr.content);
      }
      mapping.append(m);
    }
  }
}

void Cartridge::xml_parse_msu1(xml_element &root) {
  has_msu1 = true;

  foreach(node, root.element) {
    if(node.name == "map") {
      Mapping m({ &MSU1::mmio_read, &msu1 }, { &MSU1::mmio_write, &msu1 });
      foreach(attr, node.attribute) {
        if(attr.name == "address") xml_parse_address(m, attr.content);
      }
      mapping.append(m);
    }
  }
}

void Cartridge::xml_parse_serial(xml_element &root) {
  has_serial = true;
}

void Cartridge::xml_parse_link(xml_element &root) {
  has_link = true;
  link.frequency = 1;
  link.program = "";

  foreach(attr, root.attribute) {
    if(attr.name == "frequency") link.frequency = decimal(attr.content);
    if(attr.name == "program") link.program = attr.content;
  }

  foreach(node, root.element) {
    if(node.name == "map") {
      Mapping m({ &Link::read, &link }, { &Link::write, &link });
      foreach(attr, node.attribute) {
        if(attr.name == "address") xml_parse_address(m, attr.content);
      }
      mapping.append(m);
    }
  }
}

void Cartridge::xml_parse_address(Mapping &m, const string &data) {
  lstring part;
  part.split(":", data);
  if(part.size() != 2) return;

  lstring subpart;
  subpart.split("-", part[0]);
  if(subpart.size() == 1) {
    m.banklo = hex(subpart[0]);
    m.bankhi = m.banklo;
  } else if(subpart.size() == 2) {
    m.banklo = hex(subpart[0]);
    m.bankhi = hex(subpart[1]);
  }

  subpart.split("-", part[1]);
  if(subpart.size() == 1) {
    m.addrlo = hex(subpart[0]);
    m.addrhi = m.addrlo;
  } else if(subpart.size() == 2) {
    m.addrlo = hex(subpart[0]);
    m.addrhi = hex(subpart[1]);
  }
}

void Cartridge::xml_parse_mode(Mapping &m, const string& data) {
       if(data == "direct") m.mode = Bus::MapMode::Direct;
  else if(data == "linear") m.mode = Bus::MapMode::Linear;
  else if(data == "shadow") m.mode = Bus::MapMode::Shadow;
}

Cartridge::Mapping::Mapping() {
  mode = Bus::MapMode::Direct;
  banklo = bankhi = addrlo = addrhi = offset = size = 0;
}

Cartridge::Mapping::Mapping(Memory &memory) {
  read = { &Memory::read, &memory };
  write = { &Memory::write, &memory };
  mode = Bus::MapMode::Direct;
  banklo = bankhi = addrlo = addrhi = offset = size = 0;
}

Cartridge::Mapping::Mapping(const function<uint8 (unsigned)> &read_, const function<void (unsigned, uint8)> &write_) {
  read = read_;
  write = write_;
  mode = Bus::MapMode::Direct;
  banklo = bankhi = addrlo = addrhi = offset = size = 0;
}

#endif
