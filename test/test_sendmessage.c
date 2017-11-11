#include <telebot.h>

Bot * _bot;

int main(int argc, char *argv[]){
	telebot_init();

	if(argc != 2)
		fprintf(stderr, "sendphoto <token>");

    _bot = telebot(argv[1]);

/* Message * send_photo_chat(Bot * bot, long int chat_id, char * filename,
			  char * caption, bool disable_notification,
			  long int reply_to_message_id){
*/
	Message * message = send_message_chat(_bot, 100856717, "Testan\ndoooo", MODEMARKDOWN, 0, 0, 0);


	if(message){
		printf("enviado");
	}
	else{
		Error * error = show_error();
		if(error)
			printf("ec=%ld d=%s", error->error_code, error->description);
	}

	return 0;
}

