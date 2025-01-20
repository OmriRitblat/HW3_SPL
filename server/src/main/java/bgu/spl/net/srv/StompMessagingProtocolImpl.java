package bgu.spl.net.srv;
import bgu.spl.net.Frames.Connect;
import bgu.spl.net.Frames.Frame;
import bgu.spl.net.Frames.Message;
import bgu.spl.net.api.StompMessagingProtocol;

public class StompMessagingProtocolImpl implements StompMessagingProtocol<String>{
    private boolean shouldTerminate = false;
    private int connectionId;
    private Connections<String> connections;

    public StompMessagingProtocolImpl() {
        this.connections = null;
        this.connectionId = -1;
    }
    @Override
    public void start(int connectionId, Connections<String> connections) {
        this.connectionId = connectionId;
        this.connections = connections;
    }

    @Override
    public void process(String message) {
        String[] lines = message.split("\n");
        String type=lines[0];
        Frame f;
        if(type.equals("CONNECT")) {
            f=new Connect(message);
        }
        else if(type.equals("MESSAGE")){
            f=new Message(message);
        }
        else{
            f=new Message(message);
            //TODO implement in all type frames
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
