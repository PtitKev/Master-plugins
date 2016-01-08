
#include "Kernel.h"
#include "Float.h"

namespace ydleMaster {

class frame_ydle : public IFrame
{
  public:
    uint8_t type;
    uint8_t crc;
    void Dump (const char * msg)
    {
      char sztmp[255];

      if (msg)
        YDLE_DEBUG << "\t" << msg ;

      YDLE_DEBUG << "Emetteur : " << (int) sender;
      YDLE_DEBUG << "Recepteur : " << (int) receptor;
      YDLE_DEBUG << "Type : " << (int) type;
      YDLE_DEBUG << "Taille : " << (int) taille;
      YDLE_DEBUG << "Crc : " << (int) crc;

      sprintf(sztmp, "Data Hex : ");
      for (int a=0; a < taille - 1; a++)
        sprintf(sztmp,"%s 0x%02X", sztmp, data[a]);
      YDLE_DEBUG << sztmp;

      sprintf(sztmp, "Data Dec : ");
      for (int a=0; a < taille - 1; a++)
        sprintf(sztmp,"%s %d", sztmp, data[a]);
      YDLE_DEBUG << sztmp;
    }
  private:
    std::string frameBits;
} ;

} ; // namespace ydleMaster