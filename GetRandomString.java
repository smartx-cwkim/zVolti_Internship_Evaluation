import java.util.Random;

class GetRandomString {
	private int length;
	private char[] charactors = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u',
	        'v','w','x','y','z'};
	private StringBuffer sb = new StringBuffer();
	private Random rn = new Random();
	
	
	public GetRandomString(int length) {
		this.length = length;
		
		for(int i=0;i<length;i++) {
			sb.append(charactors[rn.nextInt(charactors.length)]);
		}
	}
	
	public String get() {
		return sb.toString();
	}
}
