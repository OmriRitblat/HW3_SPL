package bgu.spl.net.Frames.Res;

import java.util.concurrent.ConcurrentHashMap;

public class Reciept extends ResponseFrame {
    private int recieptId;

    public Reciept(String type, ConcurrentHashMap<String, String> headers, String body) {
        super(type,headers, body);
        this.recieptId = recieptId;
    }
    public Reciept(int recieptId) {
        super("RECEIPT", ResponseFrame.getData("RECEIPT/n receipt-id :" + recieptId+"\n\n"),ResponseFrame.getBody("CONNECTED/n version:" + recieptId+"\n\n"));
        this.recieptId = recieptId;
    }
}
