package pongLobby;

import java.io.IOException;
import java.lang.Thread;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;



public class UnicastListener extends Thread {
	UnicastListener(Lobby lobby){
		_lobby=lobby;
	}
	
	public void run(){
		_initializeSocket();	
		
		while(_lobby.get_state().equals(LobbyState.WAITING))
		{
			_readPacket();
		}
		
		if(_lobby.get_state().equals(LobbyState.STARTING))
			_sendGoToAll();
	}
	
	
	//Méthodes privées
	//*******************
	
	private void _sendGoToAll()
	{
		for(int i=0;i<_lobby.get_playersVector().size();i++)
		{
			String str="GO " + _lobby.get_serverPort() + " " +
					_lobby.get_serverAddress().toString();
			str+="\0";
	 		DatagramPacket packet= new DatagramPacket(str.getBytes(), str.length(),
					_lobby.get_playersVector().elementAt(i).get_ipAddress(),
					_lobby.get_playersVector().elementAt(i).get_port());
			try{
				_socket.send(packet);
			}
			catch(IOException exception)
			{
				System.out.println("Impossible d'envoyer le paquet dans la socket: "
						+ exception);
				System.exit(1);
			}	
		}
	}
	
	private void _initializeSocket()
	{
		
		try{
			_socket = new DatagramSocket(_lobby.get_unicastPort());
		}
		catch(IOException exception)
		{
			System.out.println("Impossible de lancer la socket: " + exception);
			System.exit(1);
		}
	}
	
	private void _sendString(String msg, InetAddress address, int port){
		String str=msg;
		str+="\0";
 		DatagramPacket packet= new DatagramPacket(str.getBytes(), str.length(),
				address, port);
		try{
		_socket.send(packet);
		}
		catch(IOException exception)
		{
			System.out.println("Impossible d'envoyer le paquet dans la socket: "
					+ exception);
			System.exit(1);
		}
	}

	private void _readPacket(){
		byte[] data = new byte[200];
		Player player = null;
		DatagramPacket packet = new DatagramPacket(data, data.length);
		try{
			_socket.receive(packet);
		}
		catch(IOException exception)
		{
			System.out.println("Impossible de lire le paquet dans la socket: "
					+ exception);
		}
		String str = new String(packet.getData(), 0, packet.getLength());
		System.out.println(str);
		String[] strl= str.split(" ");
		if(strl[0].equals("ACK"))
		{
			long id=0;
			try{
				id= Long.parseLong(strl[1]);
			}
			catch(NumberFormatException ex)
			{
				System.out.println(ex);
				System.exit(1);
			}
			player = _lobby.searchPlayer(id);
			if(player != null)
			{
				_sendString("SYNC " + player.get_id(), player.get_ipAddress(), player.get_port());
				player.resetTimer();
			}

		}
	}
	//Attributs privés
	//****************
	private Lobby _lobby;
	private DatagramSocket _socket;
}
