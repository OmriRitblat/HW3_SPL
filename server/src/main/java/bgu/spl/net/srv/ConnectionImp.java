package bgu.spl.net.srv;

import java.util.List;
import java.util.concurrent.ConcurrentHashMap;

public class ConnectionImp<T> {
    private ConcurrentHashMap<Integer, ConnectionHandler> clients;
    private ConcurrentHashMap<String, List<Integer>> subsribtion;

    public boolean send(int connectionId, T msg){
        return true;
    }

    public void send(String channel, T msg){

    }

    public void disconnect(int connectionId){

    }
}
