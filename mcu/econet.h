#include <stdint.h>

struct econet_frame
{
  uint8_t *buffer;
  uint8_t *data;
  size_t length;
};
