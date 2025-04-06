//----------------------------------------------------------------------------
// Copyright (c) 2015 Shawn Chidester <zd3nik@gmail.com>, All rights reserved
//----------------------------------------------------------------------------

#include "senjo/UCIAdapter.h"
#include "senjo/Output.h"
#include "Bitfoot.h"

//----------------------------------------------------------------------------
int main(int /*argc*/, char** /*argv*/)
{
  bitfoot::Bitfoot engine;
  senjo::UCIAdapter adapter;

  if (!adapter.Start(engine)) {
    senjo::Output() << "Unable to start UCIAdapter";
    return 1;
  }

  char sbuf[16384];
  memset(sbuf, 0, sizeof(sbuf));

  while (fgets(sbuf, sizeof(sbuf), stdin)) {
    char* cmd = sbuf;
    senjo::NormalizeString(cmd);
    if (!adapter.DoCommand(cmd)) {
      break;
    }
  }

  return 0;
}
