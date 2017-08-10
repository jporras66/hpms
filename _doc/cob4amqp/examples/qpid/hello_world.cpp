#include <qpid/messaging/Connection.h>
#include <qpid/messaging/Message.h>
#include <qpid/messaging/Receiver.h>
#include <qpid/messaging/Sender.h>
#include <qpid/messaging/Session.h>

#include <iostream>

using namespace qpid::messaging;

int main(int argc, char** argv) {
    std::string broker = argc > 1 ? argv[1] : "localhost:5672";
    //std::string address = argc > 2 ? argv[2] : "amq.topic";
    std::string address = argc > 2 ? argv[2] : "REQ.SV1.TIMEOUT_1";
    std::string connectionOptions = argc > 3 ? argv[3] : "";

    try {
        Connection connection(broker, connectionOptions);
        connection.open();
        Session session = connection.createSession();

        Receiver receiver = session.createReceiver(address);
        Sender sender = session.createSender(address);

        Message message;
        for (int i=0; i<10;i++){
            message.setContentObject("Hello world!");
            message.getContentObject().setEncoding("utf8");
            sender.send(message);
        }


        //message = receiver.fetch(Duration::SECOND * 1);
        while (true){
            message = receiver.fetch();
            std::cout << message.getContent() << std::endl;
            session.acknowledge();
        }


        connection.close();
        return 0;
    } catch(const std::exception& error) {
        std::cerr << error.what() << std::endl;
        return 1;
    }
}