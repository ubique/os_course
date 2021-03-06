// Created by Vadim on 22/04/2018.


#ifndef HW5_CLIENT_SERVER_MESSAGEUNIT_H
#define HW5_CLIENT_SERVER_MESSAGEUNIT_H

#include <iostream>
#include "Socket.h"
#include <future>
#include <memory>
#include <unordered_map>
#include <vector>
#include <set>
#include <map>
#include <functional>
#include <mutex>
#include <algorithm>

// Include what you use

size_t StringToInt(const std::string& s);
// Функция ToInt, а возвращает size_t?
std::string IntBytes(size_t length);
// Функция IntBytes, а принимает size_t?

// Помимо этого между функциями не чувствуется симметрии(а она должна быть выражена явно)

class MessageUnit {
 public:
  enum ClientState {
    AVAILABLE, GOT_CHAT_REQUESTS, IN_CHAT, DISCONNECTED
  };
 protected:
  enum MessageType {
    REGISTER,
    EXIT,
    SESSION,
    MESSAGE,
    GET_LIST_OF_CLIENTS,
    CREATE_CHAT,
    EXIT_CHAT,
    CREATE_CHAT_ACKNOLAGEMENT,
    KEEP_ALIVE_REQUEST,
    KEEP_ALIVE_ANSWER,
    NO_MESSAGES_LEFT,
    DISCONNECT,
    CLIENT_LIST,
    UPDATE_CHAT_REQUEST,
    CHAT_REQUESTS,
    CHOSEN_PARTNER,
    UPDATE_MSGS
  };
  
  template<typename Message>
  Message ReceiveMessage(const std::shared_ptr<Socket>& client) {
    auto length_bytes = client->ReadPacket(4);
    auto length = StringToInt(length_bytes);
    auto msg_bytes = client->ReadPacket(length);
    // why not const auto?
    Message message;
    message.ParseFromString(msg_bytes);
    return message;
  }
  
  template<typename Message>
  Message ReceiveMessageAndType(std::shared_ptr<Socket> client) {
    ReceiveMessageType(client);
    return ReceiveMessage<Message>(client);
  }
  
  MessageType ReceiveMessageType(const std::shared_ptr<Socket>& client) {
    MessageType msg_type;
    auto pack1 = client->ReadPacket(1);
    auto msg_type_encoded = (int) pack1[0];
    switch (msg_type_encoded) {
      case 0:
        msg_type = REGISTER;
        break;
      case 1:
        msg_type = MESSAGE;
        break;
      case 2:
        msg_type = EXIT;
        break;
      case 3:
        msg_type = SESSION;
        break;
      case 4:
        msg_type = GET_LIST_OF_CLIENTS;
        break;
      case 5:
        msg_type = CREATE_CHAT;
        break;
      case 6:
        msg_type = EXIT_CHAT;
        break;
      case 7:
        msg_type = CREATE_CHAT_ACKNOLAGEMENT;
        break;
      case 8:
        msg_type = KEEP_ALIVE_REQUEST;
        break;
      case 9:
        msg_type = KEEP_ALIVE_ANSWER;
        break;
      case 10:
        msg_type = NO_MESSAGES_LEFT;
        break;
      case 11:
        msg_type = DISCONNECT;
        break;
      case 12:
        msg_type = CLIENT_LIST;
        break;
      case 13:
        msg_type = UPDATE_CHAT_REQUEST;
        break;
      case 14:
        msg_type = CHAT_REQUESTS;
        break;
      case 15:
        msg_type = CHOSEN_PARTNER;
        break;
      case 16:
        msg_type = UPDATE_MSGS;
        break;
      default:
        msg_type = DISCONNECT;
        break;
    }
    return msg_type;
  }
  
  void
  SendMessageType(const std::shared_ptr<Socket>& client, MessageType msg_type) {
    int msg_type_code;
    switch (msg_type) {
      case REGISTER:
        msg_type_code = 0;
        break;
      case MESSAGE:
        msg_type_code = 1;
        break;
      case EXIT:
        msg_type_code = 2;
        break;
      case SESSION:
        msg_type_code = 3;
        break;
      case GET_LIST_OF_CLIENTS:
        msg_type_code = 4;
        break;
      case CREATE_CHAT:
        msg_type_code = 5;
        break;
      case EXIT_CHAT:
        msg_type_code = 6;
        break;
      case CREATE_CHAT_ACKNOLAGEMENT:
        msg_type_code = 7;
        break;
      case KEEP_ALIVE_REQUEST:
        msg_type_code = 8;
        break;
      case KEEP_ALIVE_ANSWER:
        msg_type_code = 9;
        break;
      case NO_MESSAGES_LEFT:
        msg_type_code = 10;
        break;
      case DISCONNECT:
        msg_type_code = 11;
        break;
      case CLIENT_LIST:
        msg_type_code = 12;
        break;
      case UPDATE_CHAT_REQUEST:
        msg_type_code = 13;
        break;
      case CHAT_REQUESTS:
        msg_type_code = 14;
        break;
      case CHOSEN_PARTNER:
        msg_type_code = 15;
        break;
      case UPDATE_MSGS:
        msg_type_code = 16;
        break;
      default:
        msg_type_code = 11;
        break;
    }
    std::string type_string{(char) msg_type_code};
    client->WriteBytes(type_string);
  }
  
  template<typename Message>
  void SendMessage(
          std::shared_ptr<Socket> client,
// Зачем здесь передача по значению?
          const Message& message,
          MessageType message_type
  ) {
    auto serialized_msg = message.SerializeAsString();
    auto length = serialized_msg.size();
    SendMessageType(client, message_type);
    client->WriteBytes(IntBytes(length));
    client->WriteBytes(serialized_msg);
  }
};

// Есть мнение что наследование имплементации - зло.

#endif //HW5_CLIENT_SERVER_MESSAGEUNIT_H
