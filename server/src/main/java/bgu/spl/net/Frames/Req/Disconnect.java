package bgu.spl.net.Frames.Req;

import bgu.spl.net.Frames.Res.Reciept;
import bgu.spl.net.srv.ConnectionImp;

public class Disconnect extends RequestFrame {
    public Disconnect(String msg) {
        super(-1);
        String[] lines = msg.split("\n");
        int colonIndex = msg.indexOf(":");
        setRecipet(Integer.parseInt(msg.substring(colonIndex + 1)));
    }

    @Override
    public void process(int id, ConnectionImp c) {

    }
}
