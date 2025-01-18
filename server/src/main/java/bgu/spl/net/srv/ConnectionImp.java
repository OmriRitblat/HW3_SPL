package bgu.spl.net.srv;

import java.util.List;
import java.util.concurrent.ConcurrentHashMap;

public class ConnectionImp<T> {
    private ConcurrentHashMap<Integer, ConnectionHandler> clients; //<connction id, connection handler>
    private ConcurrentHashMap<String, List<Integer>> subsribtion; //<channel name, list<connection id>>

    public boolean send(int connectionId, T msg){
        ConnectionHandler c=clients.get(connectionId)
        if(c!=null)
            return c.send(msg);
        return false;
    }

    public void send(String channel, T msg){
        List<Integer> sub=subsribtion.get(channel);
        if(sub!=null)
            for(Integer i:sub)
                send(i,msg);
    }

    public void disconnect(int connectionId){
        clients.remove(connectionId);
        //should remove from subscibtion also?
    }

    public void addConnect(int connectionId,ConnectionHandler ch){
        clients.put(connectionId,ch);
    }
    
}
