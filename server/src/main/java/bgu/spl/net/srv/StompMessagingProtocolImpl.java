package bgu.spl.net.srv;
import bgu.spl.net.Frames.Req.*;
import bgu.spl.net.Frames.Res.Message;
import bgu.spl.net.api.StompMessagingProtocol;

public class StompMessagingProtocolImpl implements StompMessagingProtocol<String>{
    private boolean shouldTerminate = false;
    private int connectionId;
    private ConnectionImp<String> connections;

    public StompMessagingProtocolImpl() {
        this.connections = null;
        this.connectionId = -1;
    }
    @Override
    public void start(int connectionId, Connections<String> connections) {
        this.connectionId = connectionId;
        this.connections =(ConnectionImp<String>) connections;
    }

    @Override
    public void process(String message) {
        String[] lines = message.split("\n");
        String type=lines[0];
        RequestFrame f;
        if(type.equals("CONNECT")) {
            f=new Connect(message);
        }
        else if(type.equals("SEND")){
            f=new Send(message);
        }
        else if(type.equals("DISCONNECT")){
            f=new Disconnect(message);
        }
        else if(type.equals("SUBSCRIBE")){
            f=new Subscribe(message);
        }
        else{
            f=new Unsubscribe(message);
        }
        f.process(connectionId,connections);
    }

    @Override
    public boolean shouldTerminate() {
        return shouldTerminate;
    }

    public void setShouldTerminate(boolean shouldTerminate) {
        this.shouldTerminate = shouldTerminate;
    }
}
