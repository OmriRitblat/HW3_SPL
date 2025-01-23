package bgu.spl.net.Frames.Req;

import bgu.spl.net.Frames.Frame;
import bgu.spl.net.Frames.Res.Error;
import bgu.spl.net.Frames.Res.Reciept;
import bgu.spl.net.Frames.Res.ResponseFrame;
import bgu.spl.net.srv.ConnectionImp;

public class Disconnect extends RequestFrame {
    public Disconnect(String msg) {
        super(-1,msg);
        String[] lines = msg.split("\n");
        int colonIndex = msg.indexOf(":");
        setRecipet(Integer.parseInt(msg.substring(colonIndex + 1)));
    }

    @Override
    public void process(int id, ConnectionImp c) {
        ResponseFrame f;
        if(!c.isLoggedIn(id)){
            f=new Error("user not loged in so can not be disconnected", this.getMessage(), "user not loged in so can not be disconnected", id);
            c.send(id, f);
        }
        else{
            f=new Reciept(getReciept());
            c.send(id, f);
            c.disconnect(id);
        }
        
    }
}
