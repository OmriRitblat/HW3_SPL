package bgu.spl.net.impl.stomp;

import bgu.spl.net.srv.MessageEncoderDecoderImpl;
import bgu.spl.net.srv.Reactor;
import bgu.spl.net.srv.Server;
import bgu.spl.net.srv.StompMessagingProtocolImpl;

public class StompServer {

    public static void main(String[] args) {
        if (args.length < 2) {
            System.out.println("Usage: java -jar StompServer.jar <port> <tpc/reactor>");
            return;
        }

        int port;
        String serverType;

        try {
            port = Integer.parseInt(args[0]);
            serverType = args[1].toLowerCase();

        } catch (NumberFormatException e) {
            System.out.println("Invalid port number: " + args);
            return;
        }
        
        if (!(serverType.equals("tpc") || serverType.equals("reactor"))){
            System.out.println("Invalid server type, must be 'tpc' or 'reactor'.");
            return;
        }

        Server<String> server;

        if (serverType.equals("tpc")) {
            server =Server.threadPerClient(
                port,
                    StompMessagingProtocolImpl::new,
                    MessageEncoderDecoderImpl::new
            );
            server.serve();
        }
        else if(serverType.equals("reactor")){
            server = Server.reactor(
                    Runtime.getRuntime().availableProcessors(),
                    port,
                    () -> new StompMessagingProtocolImpl(),
                    () -> new MessageEncoderDecoderImpl()
            );
            server.serve();
        }
    }
}
