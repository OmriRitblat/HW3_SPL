package bgu.spl.net.Frames.Req;

import bgu.spl.net.srv.ConnectionImp;

public abstract class RequestFrame {
    private int reciept;
    private String msg;

    public RequestFrame(int reciept, String msg) {
        this.reciept = reciept;
        this.msg=msg;
    }

    public abstract void process(int id, ConnectionImp c);

    public void setRecipet(int reciept) {
        this.reciept = reciept;
    }

    public int getReciept() {
        return reciept;
    }
    public String getMessage(){
        return msg;
    }

}
