/* broken -- need to port libstring to bstring */

#include "libbase.h"
#include "libstring.h"
#include "libstring.cpp"

FILE *fp;

string data;
stringarray line, part, subpart, output_op;

struct OpList {
  stringarray name, arg;
} op_list[64];

int32 op_count, line_num;

void clear_op_list() {
  op_count = 0;
  for(int i = 0; i < 64; i++) {
    strcpy(op_list[i].name, "");
    for(int l = 0; l < 8; l++) {
      strcpy(op_list[i].arg[l], "");
    }
  }
}

void gen_begin() {
int i = line_num;
char t[4096];
  clear_op_list();
  while(1) {
  int z = op_count++;
    strcpy(part, line[i]);
    strrtrim(part, "),");
    strrtrim(part, ") {");
    split(subpart, "(", part);
    strcpy(op_list[z].name, subpart[0]);
    split(part, ", ", subpart[1]);
    for(int l = 0; l < count(part); l++) {
      strcpy(op_list[z].arg[l], part[l]);
    }
    if(strend(line[i], " {") == true)break;
    i++;
  }

  strcpy(output_op, "//$$\r\ncase $0: {\r\n");
  line_num = i + 1;
}

void update_line(int i) {
  replace(line[i], "end;",  "break;");
}

void gen_op() {
int i = line_num, n, c;
char t[4096];
  while(1) {
    if(!strcmp(line[i], "}"))break;

  //remove cycle number
    n = strdec(line[i]);
    sprintf(t, "%d:", n);
    strltrim(line[i], t);
  //sprintf(t, "//%d:\r\n", n);
  //strcat(output_op, t);

    update_line(i);
    if(strcmp(line[i], "")) {
      strcat(output_op, "  ");
      strcat(output_op, line[i]);
      strcat(output_op, "\r\n");
    }

    i++;
    while(1) {
      if(strptr(line[i])[1] == ':' || strptr(line[i])[2] == ':' || line[i] == "}")break;

      update_line(i);
      strcat(output_op, line[i]);
      strcat(output_op, "\r\n");

      i++;
    }
  }

  strcat(output_op, "} break;");
  line_num = i + 1;
}

void gen_end() {
string t;
  for(int i = 0; i < op_count; i++) {
    strcpy(t, output_op);
    replace(t, "$$", op_list[i].name);
    replace(t, "$0", op_list[i].arg[0]);
    replace(t, "$1", op_list[i].arg[1]);
    replace(t, "$2", op_list[i].arg[2]);
    replace(t, "$3", op_list[i].arg[3]);
    replace(t, "$4", op_list[i].arg[4]);
    replace(t, "$5", op_list[i].arg[5]);
    replace(t, "$6", op_list[i].arg[6]);
    replace(t, "$7", op_list[i].arg[7]);
    fprintf(fp, "%s\r\n\r\n", strptr(t));
  }
}

void generate(char *dest, char *src) {
  fp = fopen(src, "rb");

  fseek(fp, 0, SEEK_END);
int fsize = ftell(fp);
  fseek(fp, 0, SEEK_SET);
char *buf = (char*)malloc(fsize + 1);
  fread(buf, 1, fsize, fp);
  fclose(fp);
  buf[fsize] = 0;

  strcpy(data, buf);
  free(buf);
  replace(data, "\r\n", "\n");
  split(line, "\n", data);

  fp = fopen(dest, "wb");

  line_num = 0;
  while(line_num < count(line)) {
    while(line_num < count(line) && !strcmp(line[line_num], ""))line_num++;
    if(line_num >= count(line))break;

    gen_begin();
    gen_op();
    gen_end();
  }

  fclose(fp);
}
