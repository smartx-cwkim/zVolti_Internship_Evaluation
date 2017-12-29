import java.net.*;
import java.util.LinkedList;
import java.util.Queue;
import java.util.Scanner;
import java.util.Stack;
import java.io.*;

public class testClient {
	public static void main(String[] args) {
		Socket socket = null;
		GetData gd = null;
		OutData od = null;
		int port;
		String address = null;
		
		try {
			address = args[0]; // main함수 실행과 동시에 받는 인자
			port = Integer.parseInt(args[1]);  // main함수 실행과 동시에 받는 인자(문자열을 int형으로 파싱.)
			socket = new Socket(address, port);
			
			gd = new GetData(socket);
			od = new OutData(socket);
			
			gd.start();
			od.start();
			
		} catch(Exception e) {
		}
	}
}

// 네트워크로부터 데이터를 받아서 모니터로 출력.
class GetData extends Thread {
	private BufferedReader br = null;
	private Socket socket = null;
	private PrintWriter pw = null;
	
	public GetData(Socket socket) {
		try {
			InputStream in = socket.getInputStream();
			this.br = new BufferedReader(new InputStreamReader(in));
			
			OutputStreamWriter osw = new OutputStreamWriter(System.out);
			this.pw = new PrintWriter(osw);
		} catch (Exception e) {
		}
	}
	
	public void run() {
		try {
			String str = null;
			
			while((str = this.br.readLine())!=null) {
				this.pw.println("서버로부터 들어온 문자열 : " + str);
				this.pw.flush();
			}
			
			this.interrupt();
		} catch(Exception e) {
		}
	}
}

class OutData extends Thread {
	private PrintWriter pw = null;
	private BufferedReader key = null;
	
	public OutData(Socket socket) {
		try {
			OutputStream os = socket.getOutputStream();
			this.pw = new PrintWriter(new OutputStreamWriter(os));
			
			InputStreamReader isr = new InputStreamReader(System.in);
			
			this.key = new BufferedReader(isr);
		} catch(Exception e) {
		}
	}
	
	public void run() {
		try {
			String str = null;
			Scanner scan = new Scanner(System.in);
			System.out.println("Queue, Stack을 입력하세요, Queue=1, Stack=2");
			int i = scan.nextInt();
			
			System.out.println("전송을 시작하세요 ('quit' 입력시 전송 종료)");
			
			
			// 1일 경우에는 Queue에 대한 내용을 소개.
			if(i==1) {
				Queue<String> queueStr = new LinkedList<String>(); 

				while((str = key.readLine()) != null) {
					if(str.equals("quit")) {
						break;
					}
					this.pw.println(str);
					this.pw.flush();
								
					queueStr.add(str);
				}
									
				while(!queueStr.isEmpty()) {
					System.out.println("서버로 보낸 문자열(Queue): " + queueStr.poll());
				}
				
				this.interrupt();
			} else {
				// 2의 경우에는 Stack에 대한 내용 소개.
				Stack<String> stackStr = new Stack<String>(); 
							
				while((str = key.readLine()) != null) {
					if(str.equals("quit")) {
						break;
					}
					this.pw.println(str);
					this.pw.flush();
								
					stackStr.push(str);
				}
							
				while(!stackStr.isEmpty()) {
					System.out.println("서버로 보낸 문자열(Stack): " + stackStr.pop());
				}
				
				this.interrupt();
			}
			
		} catch(Exception e) {
		} finally {
			try {
				pw.close();
				key.close();
			} catch(Exception e) {
				System.out.println("Close Error");
			}
		}
	}
}