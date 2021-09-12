#include <Arduino.h>
#include <fabgl.h>

fabgl::VGA16Controller DisplayController;
fabgl::Terminal Terminal;
fabgl::TerminalController Controller(&Terminal);

int rows;
int columns;

const char idleHeader[] = "WAITING FOR A COIN...";

const char privateKeyHeader[] = "PRIVATE KEY: ";
const char publicKeyHeader[] = "PUBLIC KEY: ";
const char addressHeader[] = "ADDRESS: ";
const char balanceHeader[] = "BALANCE: ";

void slowPrintf(const char * format, ...) {
  va_list ap;
  va_start(ap, format);
  int size = vsnprintf(nullptr, 0, format, ap) + 1;
  if (size > 0) {
    va_end(ap);
    va_start(ap, format);
    char buf[size + 1];
    vsnprintf(buf, size, format, ap);
    for (int i = 0; i < size; ++i) {
      Terminal.write(buf[i]);
      delay(15);
    }
  }
  va_end(ap);
}

void resetUI() {
  // disable cursor while clearing
  Terminal.enableCursor(false);
  // background: black, foreground: green
  Terminal.write("\e[40;92m");
  // clear screen and move cursor to 1,1
  Terminal.clear(true);
  Terminal.write("\e[2J");
  Terminal.write("\e[1;1H");
  // re-enable cursor after reset
  Terminal.enableCursor(true);
}

void initUI() {
  DisplayController.begin(
    (gpio_num_t)VGA_R1,
    (gpio_num_t)VGA_R0,
    (gpio_num_t)VGA_G1,
    (gpio_num_t)VGA_G0,
    (gpio_num_t)VGA_B1,
    (gpio_num_t)VGA_B0,
    (gpio_num_t)VGA_HSYNC,
    (gpio_num_t)VGA_VSYNC
  );
  DisplayController.setResolution(VGA_640x200_70Hz);
  // TODO: check with given screen
  DisplayController.moveScreen(-1, 2);
  Terminal.begin(&DisplayController);
  Terminal.enableCursor(true);
  rows = Terminal.getRows();
  columns = Terminal.getColumns();
  resetUI();
}


void printIdle() {
  int size = sizeof(idleHeader) / 2;
  Controller.setCursorPos(columns / 2 - size, rows/2);
  slowPrintf(idleHeader);
}

void printPick(char* privateKey, char* publicKey, char* address, char* balance) {
  // red color
  Terminal.write("\e[31m");
  slowPrintf(privateKeyHeader);
  // green color
  Terminal.write("\e[32m");
  slowPrintf(privateKey);
  Terminal.write("\r\n");

  // red color
  Terminal.write("\e[31m");
  slowPrintf(publicKeyHeader);
  // green color
  Terminal.write("\e[32m");
  slowPrintf(publicKey);
  Terminal.write("\r\n");

  // red color
  Terminal.write("\e[31m");
  slowPrintf(addressHeader);
  // green color
  Terminal.write("\e[32m");
  slowPrintf(address);
  Terminal.write("\r\n");

  // red color
  Terminal.write("\e[31m");
  slowPrintf(balanceHeader);
  // green color
  Terminal.write("\e[32m");
  slowPrintf(strcat(balance, " ETH"));
  Terminal.write("\r\n\r\n");
}