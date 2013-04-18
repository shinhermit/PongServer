package pongLobby;


public class Player {
	Player(int id){
	set_id(id);
	}
	
	//Getters/setters
	//*******************************************
	
	public int get_id() {
		return _id;
	}
	public void set_id(int _id) {
		this._id = _id;
	}
	
	// Privates fields
	//********************************************
	private int _id;
}
