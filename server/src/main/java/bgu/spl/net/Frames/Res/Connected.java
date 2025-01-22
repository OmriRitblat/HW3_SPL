package bgu.spl.net.Frames.Res;

public class Connected extends ResponseFrame {
    private String version;
    public Connected(String version) {
        super("CONNECTED");
        this.version = version;
    }
}
