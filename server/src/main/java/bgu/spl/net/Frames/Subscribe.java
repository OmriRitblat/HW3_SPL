package bgu.spl.net.Frames;

import bgu.spl.net.srv.ConnectionImp;

public class Subscribe extends Frame {
    private String channelName;


    public Subscribe(String msg) {
        super(-1);
    }

    @Override
    public void process(int id, ConnectionImp c) {
        c.addSubscribtion(channelName,id);
    }
}
