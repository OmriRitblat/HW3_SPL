package bgu.spl.net.Frames.Req;

import bgu.spl.net.srv.ConnectionImp;

public abstract class RequestFrame {
    private int reciept;
    private String msg;
    private boolean isMissingData;

    public RequestFrame(int reciept, String msg) {
        this.reciept = reciept;
        this.msg=msg;
        this.isMissingData=false;
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
    public boolean isMissingData() {
        return isMissingData;
    }
    public void setMissingData(boolean isMissingData) {
        this.isMissingData = isMissingData;
    }

}
