package bgu.spl.net.Frames;

import bgu.spl.net.srv.ConnectionImp;

public abstract class Frame {
    private int reciept;

    public Frame(int reciept) {
        this.reciept = reciept;
    }

    public abstract void process(int id, ConnectionImp c);

    public void setRecipet(int reciept) {
        this.reciept = reciept;
    }

}
