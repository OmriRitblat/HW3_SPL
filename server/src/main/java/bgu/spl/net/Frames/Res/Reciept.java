package bgu.spl.net.Frames.Res;

public class Reciept extends ResponseFrame {
    private int recieptId;

    public Reciept(int recieptId, String type) {
        super(type);
        this.recieptId = recieptId;
    }
    public Reciept(int recieptId) {
        super("RECIEPT");
        this.recieptId = recieptId;
    }
}
