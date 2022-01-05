#include <signal.h>
#include <lo.h>
#include "libpixyusb2.h"

#define OSC_ADDRESS "osc.udp://localhost:4444/pixy/"

static bool run_flag = true;

static void handle_SIGINT(int unused) {
  run_flag = false;
}

static lo_address init(Pixy2 pixy) {
  printf ("Connecting to Pixy2...\n");
  if (pixy.init() != -1) {
    pixy.changeProg("color_connected_components");
    printf ("Connecting to OSC\n");
    const lo_address addr = lo_address_new_from_url(OSC_ADDRESS);
    if(addr) return addr;
  }
  printf ("Error\n");
  exit(EXIT_FAILURE);
}

static bool get_blocks(Pixy2 pixy, const lo_address addr) {
  const auto n= pixy.ccc.getBlocks();
  for (uint8_t i = 0; i < n; ++i) {
    const auto b = pixy.ccc.blocks[i];
    if (lo_send(addr, "block", "iiiiiii",
        b.m_x, b.m_y, b.m_width, b.m_height,
        b.m_signature, b.m_index, b.m_age) < 0)
      return false;
  }
  return true;
}

int main(void) {
  signal (SIGINT, handle_SIGINT);

  Pixy2 pixy;

  const lo_address addr = init(pixy);

  while(run_flag) get_blocks(pixy, addr);

  lo_address_free(addr);

  return EXIT_SUCCESS;
}
