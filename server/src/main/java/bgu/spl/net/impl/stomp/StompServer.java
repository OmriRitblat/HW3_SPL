package bgu.spl.net.impl.stomp;

public class StompServer {

    public static void main(String[] args) {
        if (args.length < 2) {
            System.out.println("Usage: java -jar StompServer.jar <port> <tpc/reactor>");
            return;
        }

        int port;
        String serverType;

        try {
            port = Integer.parseInt(args);
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
                () -> new StompMessagingProtocolImpl(),
                () -> new MessageEncoderDecoderImpl()
            );
        }
        else if(serverType.equals("reactor")){
            server = Server.reactor(
                10,
                port,
                () -> new StompMessagingProtocolImpl(),
                () -> new MessageEncoderDecoderImpl()
            );
        }
        server.serve();
    }
}
