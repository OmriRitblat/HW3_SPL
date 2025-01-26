package bgu.spl.net.Frames.Req;

import bgu.spl.net.Frames.Frame;
import bgu.spl.net.Frames.Res.Error;
import bgu.spl.net.Frames.Res.Reciept;
import bgu.spl.net.Frames.Res.ResponseFrame;
import bgu.spl.net.srv.ConnectionImp;

public class Disconnect extends RequestFrame {
    public Disconnect(String msg) {
        super(-1,msg,"Disconnect");
        setRecipet(Integer.parseInt(super.getHeaderByKey("receipt")));
    }

    @Override
    public void process(int id, ConnectionImp c) {
        ResponseFrame f;
        if (this.getReciept() == -1){
            f=new Error("part of the data is missing, please send {reciept-id} in order to DISCONNECT", this.getMessage(),"the frame missing data",this.getReciept());
        }
        else if(!c.isLoggedIn(id)){
            f=new Error("user not loged in so can not be disconnected", this.getMessage(), "user not loged in so can not be disconnected", id);
        }
        else if(!headersCheck(0))
            f = new Error("there are to much headers", this.getMessage(),
        "headers check failed", this.getReciept());
        else{
            f=new Reciept(getReciept());
        }
        c.send(id, f.toString());
        c.disconnect(id);
        
    }
}
