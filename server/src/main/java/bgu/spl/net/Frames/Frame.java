package bgu.spl.net.Frames;

import java.io.Serializable;

public abstract class Frame implements Serializable {
    private String type;

    public Frame(String type) {
        this.type = type;
    }
}
