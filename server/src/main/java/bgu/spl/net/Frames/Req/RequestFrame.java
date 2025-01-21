package bgu.spl.net.Frames.Req;

import bgu.spl.net.srv.ConnectionImp;

public abstract class RequestFrame {
    private int reciept;

    public RequestFrame(int reciept) {
        this.reciept = reciept;
    }

    public abstract void process(int id, ConnectionImp c);

    public void setRecipet(int reciept) {
        this.reciept = reciept;
    }

    public int getReciept() {
        return reciept;
    }

}
