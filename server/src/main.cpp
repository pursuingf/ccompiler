 //#include "parser.h"
 //#include "lexer.h"
 //#include "global.h"

 //int main() {
 //    string fstring; // ȫ�ֱ���
 //    ifstream file("1.cpp");
 //    if (file.is_open()) {
 //        stringstream buffer;
 //        buffer << file.rdbuf();
 //        fstring = buffer.str();
 //        cout << fstring << endl;
 //        file.close();
 //    }
 //    else {
 //        cerr << "Unable to open file!" << endl;
 //    }
 //    string a;
 //    lexer L;
 //    L.setCodeString(fstring);
 //    L.getResult(a);
 //    cout << a;
 //    /*for (auto it = L.identifierTable.begin(); it != L.identifierTable.end(); ++it) {
 //        cout << it->first << "," << it->second << endl;
 //    }
 //    cout << '|';
 //    for (auto it = L.lexicalUnit.begin(); it != L.lexicalUnit.end(); ++it) {
 //        cout  << it->first << "," << it->second << endl;
 //    }*/
 //    //parser p;
 //    //p.initializeLR1();
 //    //p.getInput(fstring);
 //    //bool fun = true;
 //    //try {// ���ÿ����׳��쳣�ĺ���
 //    //    p.analysis();
 //    //}
 //    //catch (const std::string& e) {
 //    //     �����񵽵��쳣
 //    //    std::cout << "�����쳣: " << e << std::endl;
 //    //    fun = false;
 //    //}
 //    //catch (...) {
 //    //     ���������������͵��쳣
 //    //    std::cout << "����δ֪���͵��쳣" << std::endl;
 //    //    fun = false;
 //    //}
 //    //if (fun) {
 //    //    p.getResult(a);
 //    //    cout << a;
 //    //}
 //    //
 //    return 0;
 //}
#include <iostream>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <functional>
#include "parser.h"
#include "lexer.h"
#include "global.h"

typedef websocketpp::server<websocketpp::config::asio> server;

#define LEXER_FLAG '0'
#define PARSER_FLAG '1'
#define ERROR_FLAG "0"
class utility_server {
public:
   utility_server() {
       //������־��¼����
       m_endpoint.init_asio();
       // Set the default open handler to the echo handler
       m_endpoint.set_open_handler(bind(&utility_server::open_handler, this, std::placeholders::_1));
       // Set the default message handler to the echo handler
       m_endpoint.set_message_handler(bind(&utility_server::message_handler, this, std::placeholders::_1, std::placeholders::_2));
   }

   void open_handler(websocketpp::connection_hdl hdl) {
       server::connection_ptr con = m_endpoint.get_con_from_hdl(hdl);      // �������Ӿ��������Ӷ���
       std::string path = con->get_resource();    // ���URL·��
       m_endpoint.get_alog().write(websocketpp::log::alevel::app, "Connected to path " + path);
   }

   void message_handler(websocketpp::connection_hdl hdl, server::message_ptr msg) {
       string fstring = msg->get_payload();
       string DataToSend;
       bool fun = true;
       if (fstring[0] == LEXER_FLAG) {//���дʷ�����
           lexer L;
           L.setCodeString(&fstring[1]);
           try {// ���ÿ����׳��쳣�ĺ���
               L.getResult(DataToSend);
           }
           catch (const std::string& e) {
               // �����񵽵��쳣
               std::cout << "�����쳣: " << e << std::endl;
               fun = false;
           }
           catch (...) {
               // ���������������͵��쳣
               std::cout << "����δ֪���͵��쳣" << std::endl;
               fun = false;
           }
           if (!fun)  DataToSend = ERROR_FLAG;
              
          
       }
       else if (fstring[0] == PARSER_FLAG) {
           parser p;
           p.initializeLR1();
           p.getInput(&fstring[1]);
           try {// ���ÿ����׳��쳣�ĺ���
               p.analysis();
           }
           catch (const std::string& e) {
               // �����񵽵��쳣
               std::cout << "�����쳣: " << e << std::endl;
               fun = false;
           }
           catch (...) {
               // ���������������͵��쳣
               std::cout << "����δ֪���͵��쳣" << std::endl;
               fun = false;
           }
           if (fun) {
               p.getResult(DataToSend);
           }
           else {
               DataToSend = ERROR_FLAG;
           }
       }
       cout << DataToSend << endl;
       m_endpoint.send(hdl, DataToSend, msg->get_opcode());
   }

   void run(int port) {
       m_endpoint.set_reuse_addr(true);
       m_endpoint.listen(port);
       m_endpoint.start_accept();
       m_endpoint.run();
   }
private:
   server m_endpoint;
};

int main() {
   utility_server s;
   s.run(3000);
}


