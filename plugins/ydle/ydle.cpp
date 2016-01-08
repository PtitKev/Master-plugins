/*
 * ydle.cpp
 *
 *  Created on: Dec 20, 2015
 *      Author: PtitKev
 */

#include "ydle.h"
#include "DataAccess.h"
#include "WebServer.h"
#include "../../src/WebServer.cpp"

using namespace ydleMaster ;

void ydle::Init (IConnector *connector)
{
  _connector = connector;
  InitReception ();
}

void ydle::Start ()
{
}

void ydle::InitReception ()
{
  first = true;
  rx_active = false;
  length_ok = false;
  rx_bits = 0;
  bit_count = 0;
  rx_bytes_count = 0;
  receptor = 0;
  sender = 0;
  type = 0;
  memset(m_data, 0, sizeof(m_data));
}

void ydle::onBitReceived(uint8_t bit_value)
{
  if (rx_active) {
    bit_count ++;
    // On récupère les bits et on les places dans des variables
    // 1 bit sur 2 avec Manchester
    if (bit_count % 2 == 1) {
      if (bit_count < 16)
      {
        // Les 8 premiers bits de données
        receptor <<= 1;
        receptor |= bit_value;
      }
      else if (bit_count < 32)
      {
        // Les 8 bits suivants
        sender <<= 1;
        sender |= bit_value;
      }
      else if (bit_count < 38)
      {
        // Les 3 bits de type
        type <<= 1;
        type |= bit_value;
      }
      else if (bit_count < 48)
      {
        // Les 5 bits de longueur de trame
        rx_bytes_count <<= 1;
        rx_bytes_count |= bit_value;
      }
      else if ((bit_count-48) < (rx_bytes_count * 16))
      {
        length_ok = true;
        m_data[(bit_count-48)/16] <<= 1;
        m_data[(bit_count-48)/16]|= bit_value;
      }
    }

    // Quand on a reçu les 24 premiers bits, on connait la longueur de la trame
    // On vérifie alors que la longueur semble logique	
    if (bit_count >= 48)
    {
      // Les bits 19 à 24 informent de la taille de la trame
      // On les vérifie car leur valeur ne peuvent être < à 1 et > à 31
      if (rx_bytes_count < 1 || rx_bytes_count > 31)
      {
        // Mauvaise taille de message, on ré-initialise la lecture
        YDLE_INFO << rx_bytes_count << " error !";
        InitReception ();
        return;
      }
    }

    // On vérifie si l'on a reçu tout le message
    if ((bit_count-48) >= (rx_bytes_count*16) && (length_ok == true))
    {
      YDLE_INFO << "complete";
      frame_ydle frame;
      InitFrame(&frame, sender, receptor, type) ;
      frame.taille = rx_bytes_count;
      memcpy(frame.data, m_data, rx_bytes_count - 1) ; // copy data len - crc
      // crc calcul
      frame.crc = computeCrc(&frame);
      onFrameReceived (&frame);
      InitReception ();
    }
  } else if (rx_bits == YDLE_START) {
    // Octet de start, on commence à collecter les données
    YDLE_INFO << "start received";
    rx_active = true;
    rx_bytes_count = 0;
    bit_count = 0;
  }
}

void ydle::onFrameReceived(frame_ydle *frame)
{
  frame->Dump("onFrameReceived");
  // If it's a ACK then handle it
  if(frame->type == YDLE_TYPE_ACK)
  {
    std::list<ydle::ACKCmd_t>::iterator i;
    for(i=mListACK.begin(); i != mListACK.end(); ++i)
    {
      if(frame->sender == i->Frame.receptor && frame->receptor == i->Frame.sender)
      {
        YDLE_DEBUG << "Remove ACK from pending list";
        i=mListACK.erase(i);
        break; // remove only one ACK at a time.
      }
    }
  }
  else if(frame->type == YDLE_TYPE_ETAT_ACK)
  {
    YDLE_DEBUG << "New State/ACK frame ready to be sent :";
    NotifyIHM (frame) ;
    SendACK(frame);
  }
  else if(frame->type == YDLE_TYPE_ETAT)
  {
    YDLE_DEBUG << "New State frame ready to be sent :";
    NotifyIHM (frame) ;
  }
  else if(frame->type == YDLE_TYPE_CMD)
  {
    YDLE_DEBUG << "New Command frame ready to be sent :";
    NotifyIHM (frame) ;
  }
  else
  {
    YDLE_DEBUG << "Bad frame, trash it :";
  }
}

Json::Value ydle::onIHMRequest (const WebServer::HTTPRequest *request)
{
  Json::Value reply;

	string url = request->Url();

	if(std::count(url.begin(), url.end(), '/') > 1){
		// Split the URL
		const char *pch = std::strtok((char*)url.data(), "/" );
		pch = std::strtok(NULL, "/" );
		url = pch;
	}

  if(url.compare("on") == 0){
		string nid = request->GetParameter("nid");
		string target = request->GetParameter("target");
		string sender = request->GetParameter("sender");
		if(nid.length() == 0 || target.length() == 0 || sender.length() == 0){
      reply["result"] = "KO";
      reply["message"] = "A parameter is missing";
		}else{
      frame_ydle frame;
      InitFrame(&frame, atoi(sender.c_str()), atoi(target.c_str()), YDLE_TYPE_CMD) ;
      addData(&frame, YDLE_CMD_ON, 1);
      Send(&frame) ;
      reply["result"] = "OK";
      reply["message"] = "Sended";
		}
  }
  else if(url.compare("off") == 0){
		string nid = request->GetParameter("nid");
		string target = request->GetParameter("target");
		string sender = request->GetParameter("sender");
		if(nid.length() == 0 || target.length() == 0 || sender.length() == 0){
      reply["result"] = "KO";
      reply["message"] = "A parameter is missing";
		}else{
      frame_ydle frame;
      InitFrame(&frame, atoi(sender.c_str()), atoi(target.c_str()), YDLE_TYPE_CMD) ;
      addData(&frame, YDLE_CMD_OFF, 1);
      Send(&frame) ;
      reply["result"] = "OK";
      reply["message"] = "Sended";
		}
  }
  else if(url.compare("link") == 0){
		string target = request->GetParameter("target");
		string sender = request->GetParameter("sender");
		if(target.length() == 0 || sender.length() == 0){
      reply["result"] = "KO";
      reply["message"] = "A parameter is missing";
		}else{
      frame_ydle frame;
      InitFrame(&frame, atoi(sender.c_str()), atoi(target.c_str()), YDLE_TYPE_CMD) ;
      addData(&frame, YDLE_CMD_LINK, 1);
      Send(&frame) ;
      reply["result"] = "OK";
      reply["message"] = "Sended";
		}
  }
  else if(url.compare("reset") == 0){
		string target = request->GetParameter("target");
		string sender = request->GetParameter("sender");
		if(target.length() == 0 || sender.length() == 0){
      reply["result"] = "KO";
      reply["message"] = "A parameter is missing";
		}else{
      frame_ydle frame;
      InitFrame(&frame, atoi(sender.c_str()), atoi(target.c_str()), YDLE_TYPE_CMD) ;
      addData(&frame, YDLE_CMD_RESET, 1);
      Send(&frame) ;
      reply["result"] = "OK";
      reply["message"] = "Sended";
		}
  }else{
    reply["result"] = "KO";
    reply["message"] = "Unknow command";
  }
  return reply;
}

// Ajout d'une valeur bool
void ydle::addData(frame_ydle *frame, int type, bool data)
{
  if (frame->taille < 29)
  {
    frame->data[frame->taille] = type << 4;
    frame->data[frame->taille] += YDLE_DATA_BOOL << 1;
    frame->data[frame->taille] += data & 0x0F;
    frame->taille++;
  }
}

// Ajout d'une valeur int
void ydle::addData(frame_ydle *frame, int type, int data)
{
  // 8 bits int
  if (data > -256 && data < 256)
  {
    if (frame->taille < 28) 
    {
      frame->data[frame->taille] = type << 4;
      frame->data[frame->taille] += YDLE_DATA_UINT8 << 1;
      frame->data[frame->taille] += (data < 0 ? 1 : 0) << 0;
      frame->data[frame->taille + 1] = data;
      frame->taille += 2;
    }
  }
  // 16 bits int
  else if (data > -65536 && data < 65536) 
  {
    if (frame->taille < 27)
    {
      frame->data[frame->taille] = type << 4;
      frame->data[frame->taille] += YDLE_DATA_UINT8 << 1;
      frame->data[frame->taille] += (data < 0 ? 1 : 0) << 0;
      frame->data[frame->taille + 1] = (data >> 8) & 0xFF;
      frame->data[frame->taille + 2] = data;
      frame->taille += 3;
    }
  }
}

// Ajout d'une valeur long int
void ydle::addData(frame_ydle *frame, int type, long int data)
{
  // 24 bits int
  if (data > -16777216 && data < 16777216)
  {
    if (frame->taille < 26)
    {
      frame->data[frame->taille] = type << 4;
      frame->data[frame->taille] += YDLE_DATA_UINT8 << 1;
      frame->data[frame->taille] += (data < 0 ? 1 : 0) << 0;
      frame->data[frame->taille + 1] = (data >> 16);
      frame->data[frame->taille + 2] = (data >> 8);
      frame->data[frame->taille + 3] = data;
      frame->taille += 4;
    }
  }
  // 32 bits int
  else if (data > -4294967296 && data < 4294967296)
  {
    if (frame->taille < 25)
    {
      frame->data[frame->taille] = type << 4;
      frame->data[frame->taille] += YDLE_DATA_UINT8 << 1;
      frame->data[frame->taille] += (data < 0 ? 1 : 0) << 0;
      frame->data[frame->taille + 1] = (data >> 24) & 0xFF;
      frame->data[frame->taille + 2] = (data >> 16) & 0xFF;
      frame->data[frame->taille + 3] = (data >> 8) & 0xFF;
      frame->data[frame->taille + 4] = data;
      frame->taille += 5;
    }
  }
}

void ydle::Receive (uint8_t rx_value)
{
  int iTime = getTime();
  if (iTime - iLastTime > TIMEOUT) {
    iLastTime = iTime;
    rx_bits = 0;
    return;
  }
  rx_value = !rx_value;
  int duration = (iTime - iLastTime) < DURATION + 250 ? 1 : 2;
  if (duration == 2) {
    first = !first;
    addBit(rx_value);
  }
  addBit(rx_value);
  if (rx_value) {
    pthread_create(&thread, NULL, ydle::ThreadProcess, this);
    pthread_detach(thread);
  }
  first = !first;
  iLastTime = iTime;
}

void ydle::InitFrame (frame_ydle *frame, int sender, int receptor, int type)
{
  frame->sender = sender;
  frame->receptor = receptor;
  frame->type = type;
	frame->taille=0;
	memset(frame->data, 0, sizeof(frame->data));
	frame->crc=0;
}

void ydle::Send(frame_ydle *frame)
{
  int i;
  frame->taille++;
  frame->crc = computeCrc(frame) ;

  frame->Dump("Send");

	itob((int)YDLE_START_BITS, 0, 8);
	itob(frame->receptor, 8, 8);
	itob(frame->sender, 16, 8);
	itob(frame->type, 24, 3);
	itob(frame->taille, 27, 5);
	for(i=0; i < frame->taille - 1; i++)
	{
		itob(frame->data[i], 32 + (8 * i), 8);
	}
	itob(frame->crc, 32 + (8 * i), 8);

	// Sequence AGC
	for (i=0; i < 32; i++)
	{
		SendPair(true, DURATION);
	}
  char sztmp[255];
  sprintf(sztmp, "Data bin : ");

	// Send Data
	for (i=0; i < (frame->taille + 4) * 8; i++) // data+entete+crc
  {
		SendPair(frameBits[i], DURATION);
    sprintf(sztmp,"%s%d", sztmp, frameBits[i]);
	}
  YDLE_DEBUG << sztmp;

	_connector->Send(0);
}

void ydle::SendACK(frame_ydle *frame)
{
  InitFrame(frame, frame->receptor, frame->sender, YDLE_TYPE_ACK) ;
  Send(frame) ;
}

void ydle::SendBit(bool b, int pause)
{
	if (b) {
		_connector->Send(1);
	} else {
    _connector->Send(0);
	}
  usleep(pause);
}

void ydle::SendPair(bool b, int pause) 
{
	SendBit(b, pause);
	SendBit(!b, pause);
}

void ydle::addBit(uint8_t bit)
{
  bit = bit ? 0x1 : 0x0;
  rx_bits <<= 1;
  rx_bits |= bit;
  onBitReceived(bit);
}

uint8_t ydle::computeCrc(frame_ydle *frame)
{
  uint8_t *buf, crc;
  int a, j;

  buf = (uint8_t*) malloc(frame->taille + 3);
  memset(buf, 0x0, frame->taille+3);

  buf[0] = frame->sender;
  buf[1] = frame->receptor;
  buf[2] = frame->type;
  buf[2] = buf[2] << 5;
  buf[2] |= frame->taille;

  for (a=3, j=0; j < frame->taille - 1; a++, j++)
  {
    buf[a] = frame->data[j];
  }
  YDLE_DEBUG << "buf" << buf;
  crc = crc8(buf, frame->taille + 2);
  free(buf);
  return crc;
}

int ydle::getTime(void)
{
  struct timeval localTime;
  gettimeofday(&localTime, NULL); 
  int iTime=localTime.tv_sec * 1000000;
  iTime+=localTime.tv_usec;
  return iTime;
}

void ydle::itob(unsigned long integer, int start, int length)
{
	for (int i=0; i<length; i++)
	{
		int pow2 = 1 << (length-1-i) ;
		frameBits[start + i] = ((integer & pow2) != 0);
	}
}