package bgu.spl.net.Frames;

import bgu.spl.net.srv.ConnectionImp;

public class Unsubscribe extends Frame {
    private String channelName;


    public Unsubscribe(String msg) {
        super(-1);
    }

    @Override
    public void process(int id, ConnectionImp c) {
        c.removeSubscribtion(channelName,id);
    }
}
