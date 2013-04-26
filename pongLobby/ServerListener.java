package pongLobby;

import java.io.DataOutputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public class ServerListener extends Thread {

	ServerListener(Lobby lobby){
		_lobby=lobby;
	}
	
	public void run(){
		try {
			_serverSocket= new ServerSocket(6668);
		} catch (IOException e) {
			System.out.println("Impossible de connecter la socket TCP " + e);
		}
		Socket socket=null;
		if(!_lobby.is_isServerReady())
		{
			try {
				System.out.println("En attente d'une connection");
				socket = _serverSocket.accept();
				System.out.println("Reçu une connection");
			} catch (IOException e) {
				System.out.println("Impossible de mettre en écoute la socket TCP " + e);
			}
			try {
				_outputStream=new DataOutputStream(socket.getOutputStream());
			} catch (IOException e) {
				System.out.println("Impossible de connecter le flux a la socket " + e);
			}
			
		}
		while(_lobby.get_state().equals(LobbyState.WAITING))
		{}
		if(_lobby.get_state().equals(LobbyState.STARTING))
				_sendString(Integer.toString( _lobby.get_playersVector().size()), _outputStream);
		
		
	}
	
	private void _sendString(String msg, DataOutputStream stream){
		String str=msg;
		str+="\0";
		try{
			stream.writeChars(str);
			System.out.println("Envoyé " + str);
		}
		catch(IOException exception)
		{
			System.out.println("Impossible d'envoyer le paquet dans la socket: "
					+ exception);
			System.exit(1);
		}
	}
	
	private Lobby _lobby;
	private ServerSocket _serverSocket;
	private DataOutputStream _outputStream;
	
}
