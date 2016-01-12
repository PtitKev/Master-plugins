/*
 * IFrame.h
 *
 *  Created on: Fev 24, 2014
 *      Author: fets
 */

#ifndef IFRAME_H
#define IFRAME_H

#include <string.h>

#include "YdleLog.h"

namespace ydleMaster {

class IFrame
{
  public:
    uint8_t receptor;
    uint8_t sender;
    uint8_t taille;
    uint8_t data[32];

    IFrame () {
      receptor = 0;
      sender = 0;
      memset(data, 0, sizeof(data));
    } ;
    virtual ~IFrame () {} ;
    void Dump (const char * msg)
    {
      char sztmp[255];

      if (msg)
        YDLE_DEBUG << msg ;

      YDLE_DEBUG << "Emetteur : " << (int) sender;
      YDLE_DEBUG << "Recepteur :" << (int) receptor;

      sprintf(sztmp, "Data Hex: ");
      for (int a=0; a < taille - 1; a++)
        sprintf(sztmp,"%s 0x%02X", sztmp, data[a]);
      YDLE_DEBUG << sztmp;

      sprintf(sztmp, "Data Dec: ");
      for (int a=0; a < taille - 1; a++)
        sprintf(sztmp,"%s %d", sztmp, data[a]);
      YDLE_DEBUG << sztmp;
    }
} ;

} ; // namespace ydleMaster

#endif // IFRAME_H