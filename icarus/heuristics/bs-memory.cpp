namespace Heuristics {

struct BSMemory : Heuristics {
  BSMemory(vector<uint8_t>& data, string location);
  explicit operator bool() const;
  auto manifest() const -> string;

private:
  vector<uint8_t>& data;
  string location;
};

BSMemory::BSMemory(vector<uint8_t>& data, string location) : data(data), location(location) {
}

BSMemory::operator bool() const {
  return data.size() >= 0x8000;
}

auto BSMemory::manifest() const -> string {
  if(!operator bool()) return {};

  string output;
  output.append("game\n");
  output.append("  sha256: ", Hash::SHA256(data).digest(), "\n");
  output.append("  name:   ", Location::prefix(location), "\n");
  output.append("  label:  ", Location::prefix(location), "\n");
  output.append(memory("NAND", data.size(), "program.rom"));
  return output;
}

}
