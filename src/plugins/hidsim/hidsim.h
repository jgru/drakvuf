/**
 * General Hidsim plugin description:
 * This plugin simulates mouse movements 
 */

#ifndef HIDSIM_H
#define HIDSIM_H

#include "plugins/private.h"
#include "plugins/plugins_ex.h"

struct hidsim_config
{
  const char* config_fp;
};

class hidsim: public pluginex
{
  
public:
  output_format_t format;
  const char* config_fp; 

  hidsim(drakvuf_t drakvuf, const hidsim_config* config, output_format_t output);
  ~hidsim();

private:
  void inject_mouse_movement(drakvuf_t drakvuf);
};


#endif