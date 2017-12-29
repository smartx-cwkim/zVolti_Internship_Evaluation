import java.util.Random;

class getRandomString {
	private int length;
	private char[] charactors = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u',
	        'v','w','x','y','z','0','1','2','3','4','5','6','7','8','9',' '};
	private StringBuffer sb = new StringBuffer();
	private Random rn = new Random();
	
	
	public getRandomString(int length) {
		this.length = length;
		
		for(int i=0;i<length;i++) {
			sb.append(charactors[rn.nextInt(charactors.length)]);
		}
	}
	
	public String get() {
		return sb.toString();
	}
}
