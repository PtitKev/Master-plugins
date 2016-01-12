/*
 * ydle.h
 *
 * Created on: Dec 20, 2015
 * Author: PtitKev
 */

#ifndef YDLE_H
#define YDLE_H

#include "IFrame.h"
#include "INode.h"

#include "main.h"
#include "node.h"
#include "frame.h"
#include "Crc.h"
#include "Kernel.h"
#include "Float.h"

#define DURATION 1000
#define TIMEOUT 2500
#define YDLE_MAX_SIZE_FRAME 36
#define YDLE_START 0x06559
#define YDLE_START_BITS 0x42

#define YDLE_TYPE_STATE			1 // Node send data
#define YDLE_TYPE_CMD				2 // ON/OFF sortie etc...
#define YDLE_TYPE_ACK				3 // Acquit last command
#define YDLE_TYPE_STATE_ACK		4 // Node send data and want ACK

#define YDLE_DATA_BOOL				0 // true / false (1 bit / 8 bits data)
#define YDLE_DATA_UINT8			1 // (8 bits / 16 bits data)
#define YDLE_DATA_UINT16			2 // (16 bits / 24 bits data)
#define YDLE_DATA_UINT24			3 // (24 bits / 32 bits data)

#define YDLE_CMD_LINK				0 // Link a node to the master
#define YDLE_CMD_ON					1 // Send a ON command to node data = N° output
#define YDLE_CMD_OFF				2 // Send a OFF command to node data = N° output
#define YDLE_CMD_RESET				3 // Ask a node to reset is configuration
#define YDLE_CMD_SET				4 // Set value
#define YDLE_CMD_GET				5 // Get value
#define YDLE_CMD_PING				6 // PING

namespace ydleMaster {

class ydle : public IProtocol 
{
  public:
    std::string Name() { return "Ydle" ; }
    std::string Class() { return "ydle" ; }
    std::string Connector() { return "radio" ; }
    std::string getDefaultNode() { return "node_ydle" ; }

    void Init(IConnector *) ;
    void Start() ;

    void Receive(uint8_t) ;
    void SendACK(frame_ydle *frame) ;
    void Send(frame_ydle *frame) ;

  public:
    struct ACKCmd_t
    {
      frame_ydle Frame;
      int Time;
      int iCount;
    };
    std::list<ydle::ACKCmd_t> mListACK;

    bool first;
    bool length_ok;
    bool rx_active;

    int iLastTime;
    uint16_t rx_bits;

    uint8_t bit_count;
    int rx_bytes_count;

    uint8_t receptor;
    uint8_t sender;
    uint8_t type;
  
    uint8_t m_data[YDLE_MAX_SIZE_FRAME];

    pthread_t thread;

    // Ajout d'une donnée
    void addData(frame_ydle *frame, int type, bool data) ;
    void addData(frame_ydle *frame, int type, int data) ;
    void addData(frame_ydle *frame, int type, long int data) ;
    void addData(frame_ydle *frame, int type, float data) ;

    void InitReception() ;
    void InitFrame(frame_ydle *frame, int, int, int) ;

    void onBitReceived(uint8_t) ;
    void onFrameReceived(frame_ydle *frame) ;
    Json::Value onIHMRequest(const WebServer::HTTPRequest *request) ;

    static void *ThreadProcess(void *arg) {
      usleep(TIMEOUT - 200);
      int thisTime = ((ydle*)arg)->getTime();
      int iLastTime = ((ydle*)arg)->iLastTime;
      if (thisTime - iLastTime > TIMEOUT - 200) {
        printf("Ok %d > %d\n", thisTime - iLastTime, TIMEOUT - 200);
        // timeout, impulse to 0
        ((ydle*)arg)->addBit(0);
      }
      return NULL;
    } ;

    void addBit(uint8_t) ;
    void SendBit(bool, int) ;
    void SendPair(bool, int) ;
    uint8_t computeCrc(frame_ydle *frame) ;
    void itob(unsigned long integer, int start, int length);
    int getTime(void) ;
    bool frameBits[272]; 
} ;

} ; // namespace ydleMaster

extern "C" int LoadPlugins(ydleMaster::Kernel &) ;

#endif // YDLE_H