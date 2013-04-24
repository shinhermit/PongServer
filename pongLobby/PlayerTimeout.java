package pongLobby;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class PlayerTimeout implements ActionListener {

	PlayerTimeout(Player player)
	{
		_player=player;
	}
	
	@Override
	public void actionPerformed(ActionEvent arg0) {
		_player.get_lobby().get_playersVector().remove(_player);
		_player=null;
	}
	
	private Player _player;
}
