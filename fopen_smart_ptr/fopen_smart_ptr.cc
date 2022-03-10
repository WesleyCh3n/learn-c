#include <cstring>
#include <fstream>
#include <memory>
#include <iostream>

using std::cout;
using std::string;
using std::unique_ptr;

auto fcloser = [](FILE *f) { fclose(f); };
using unique_file = std::unique_ptr<FILE, decltype(fcloser)>;

int c_write_file(const char* filename, const char *lines) {
  unique_file f(fopen(filename, "w"), fcloser);
  fwrite(lines, strlen(lines), 1, f.get());
  return 0;
}

int c_read_file(const char* filename) {
  printf("C read: \n");
  unique_file f(std::fopen(filename, "r"), fcloser);
  if (f.get() == nullptr) {
    cout << "file not found\n";
    return 1;
  }

  fseek(f.get(), 0, SEEK_END);
  size_t Filesize = ftell(f.get());
  fseek(f.get(), 0, SEEK_SET);
  printf("file_size: %zu\n", Filesize);

  auto data = std::make_unique<char[]>(Filesize);

  fread(data.get(), Filesize, 1, f.get());

  printf("data len: %zu\n", strlen(data.get()));
  for (size_t i = 0; i < Filesize; ++i) {
    printf("%c", data[i]);
  }
  printf("\n===\n");
  return 0;
}

int cpp_write_file(const char* filename, const char* lines) {
  std::fstream f(filename, std::ios::out|std::ios::binary);
  f.write(lines, strlen(lines));
  // or
  // f << lines;
  cout << "write tellp: " << f.tellp() << '\n';
  return 0;
}

int cpp_read_file(const char* filename) {
  printf("Cpp read: \n");
  std::fstream f(filename, std::ios::in|std::ios::ate|std::ios::binary);
  if (!f.is_open())
    throw std::runtime_error("file not found");
  size_t file_size = f.tellg();
  printf("file_size: %zu\n", file_size);

  char * data = new char [file_size];
  f.seekg(0, std::ios::beg);
  f.read(data, file_size);
  printf("data len: %zu\n", strlen(data));
  for (size_t i = 0; i < file_size; ++i) {
    printf("%c", data[i]);
  }
  printf("\n===\n");

  // another method: sstream
  /* std::stringstream buf;
   * f.seekg(0, std::ios::beg);
   * buf << f.rdbuf();
   * printf("%s\n", buf.str().c_str()); */

  return 0;
}

int main() {
  c_write_file("c_test.txt", "hello world\nfrom\nc");
  cpp_write_file("cpp_test.txt", "hello world\nfrom\ncpp");

  // c_read_file("c_test.txt");
  // c_read_file("cpp_test.txt");
  cpp_read_file("c_test.txt");
  cpp_read_file("cpp_test.txt");
  // cpp_read_file("c_test.txt");

  // cout << sizeof(unsigned char) << ' ' << sizeof(uint8_t) << '\n';
  // cout << sizeof(unsigned short) << ' ' << sizeof(uint16_t) << '\n';
  // cout << sizeof(unsigned int) << ' ' << sizeof(uint32_t) << '\n';
  // cout << sizeof(unsigned long) << ' ' << sizeof(uint64_t) << '\n';
  // cout << sizeof(unsigned long long) << ' ' << sizeof(uint64_t) << '\n';
  return 0;
}

