#include <telebot.h>

/* parameter parse_mode */
#define MODEHTML "HTML"
#define MODEMARKDOWN "MARKDOWN"

void telebot_init () {

//#ifndef CONFIG_DEFAULT /* read or not read config file */
//    read_config();
//#endif
    network_init();
/*    log_init();*/
}



/**
 * Authentic bot token
 */
Bot * telebot (const char *token) {

    User *bot_user = get_me(token);
    
    if (bot_user) {
        Bot *obot = bot(token, bot_user);

        return obot;
    }

    return NULL;
}



/**
 * Returns a User object of the owner bot.
 * https://core.telegram.org/bots/api#getme
 */ 
User *get_me (const char *token) {
    User * user;
    json_t *json;
    if (!token)
        return NULL;

    json = generic_method_call(token, API_GETME);
    user = user_parse(json);

    json_decref(json);

    return user;
}



/**
 * Returns the updates list
 * https://core.telegram.org/bots/api#getupdates
 */ 
Update *get_updates (Bot *bot, long int offset, long int limit,
                     long int timeout, char *allowed_updates) {

    json_t *json;
    json = generic_method_call(bot->token, API_GETUPDATES,
        offset, limit, timeout, allowed_updates );

    size_t length, i;
    length = json_array_size(json);

    Update *up = NULL, *_temp = NULL;

    if (length > 0) {
        up = update_parse(json_array_get(json, 0));

        for (i = 1; i < length; i++) {
            _temp = update_parse(json_array_get(json, i));
            if (_temp) {
                update_add(up, _temp);
            }
        }
    }

    json_decref(json);

    return up;
}


/**
 * Sends the given message to the given chat.
 * TODO:
 *  - Change the type of 'chat_id'
 * https://core.telegram.org/bots/api#sendmessage
 */
Message * send_message (Bot *bot, char * chat_id, char *text, char * parse_mode,
            bool disable_web_page_preview, bool disable_notification, long int reply_to_message_id,
            char * reply_markup) {
    Message * message;
    json_t * json;
    
    json = generic_method_call(bot->token, API_SENDMESSAGE,
        chat_id, text, parse_mode, (disable_web_page_preview > 0 ? "true" : "0"),
        (disable_notification > 0 ? "true" : "0"),
        reply_to_message_id, reply_markup);

    message = message_parse(json);

    json_decref (json);

    return message;
}

Message * send_message_chat (Bot *bot, long int chat_id, char *text, char *parse_mode,
            bool disable_web_page_preview, bool disable_notification, long int reply_to_message_id,
            char * reply_markup) {
    Message * message;
    char * cchat_id;

    cchat_id = api_ltoa(chat_id);

    message =  send_message(bot, cchat_id, text, parse_mode, disable_web_page_preview,
        disable_notification, reply_to_message_id, reply_markup);

    free(cchat_id);

    return message;
}



/**
 * Returns the Chat object of the given chat_id
 * https://core.telegram.org/bots/api#getchat
 */ 
Chat *get_chat (Bot *bot, char *chat_id) {
    Chat * chat;
    json_t *json = generic_method_call(bot->token, API_getChat, chat_id);
 
    chat = chat_parse(json);

    json_decref(json);

    return chat;
}


Chat * get_chat_chat(Bot *bot, long int chat_id){
    Chat * chat;
    char * cchat_id;

    cchat_id = api_ltoa(chat_id);

    chat = get_chat(bot, cchat_id);

    free(cchat_id);

    return chat;
}


/**
 * Changes the title of the given chat_id
 * Returns 1 in success, 0 otherwise
 * https://core.telegram.org/bots/api#setchattitle
 */
int set_chat_title (Bot *bot, char *chat_id, char *title) {
    int result;
    json_t *json;

    json = generic_method_call(bot->token, API_setChatTitle,
        chat_id, title);

    result = json_is_true(json);

    json_decref(json);

    return result;
}


int set_chat_title_chat (Bot *bot, long int chat_id, char *title) {
    int result;
    char * cchat_id;

    cchat_id = api_ltoa(chat_id);

    result = set_chat_title (bot, cchat_id, title);

    free(cchat_id);

    return result;
}


/**
 * Returns the requested ChatMember object.
 * https://core.telegram.org/bots/api#getchatmember
 */
ChatMember *get_chat_member (Bot *bot, char *chat_id, long int user_id) {
    json_t *json;
    ChatMember * chat_member;

    json = generic_method_call(bot->token, API_getChatMember,
        chat_id, user_id);

    chat_member = chat_member_parse(json);

    json_decref(json);

    return chat_member;
}


ChatMember *get_chat_member_chat (Bot *bot, long int chat_id, long int user_id) {
    ChatMember * chat_member;
    char * cchat_id;

    cchat_id = api_ltoa(chat_id);

    chat_member = get_chat_member (bot, cchat_id, user_id);

    free(cchat_id);

    return chat_member;
}


/**
 * Changes the given chat or channel description
 * https://core.telegram.org/bots/api#setchatdescription
 */
bool set_chat_description (Bot *bot, char *chat_id, char *description) {
    json_t *json;
    bool result;

    json = generic_method_call(bot->token, API_setChatDescription,
        chat_id, description);

    result = json_is_true(json);

    json_decref(json);

    return result;
}



bool set_chat_description_chat (Bot *bot, long int chat_id, char *description) {
    bool result;
    char * cchat_id;

    cchat_id = api_ltoa(chat_id);

    result = set_chat_description (bot, cchat_id, description);

    free(cchat_id);

    return result;
}



/**
 * Returns the number of members in the given chat
 *https://core.telegram.org/bots/api#getchatmemberscount
 */ 
int get_chat_members_count (Bot *bot, char *chat_id) {
    int result;
    json_t *json;

    json = generic_method_call(bot->token, API_getChatMemberCount, chat_id);
    
    result = json_integer_value(json);

    json_decref(json);

    return result;
}



int get_chat_members_count_chat (Bot *bot, long int chat_id) {
    int result;
    char * cchat_id;

    cchat_id = api_ltoa(chat_id);

    result = get_chat_members_count (bot, cchat_id);

    free(cchat_id);

    return result;
}


/**
 * Ban a chat user
 * https://core.telegram.org/bots/api#kickchatmember
 */
bool kick_chat_member (Bot *bot, char *chat_id, long int user_id, char *until_date) {
    bool result;
    json_t *json;

    json = generic_method_call(bot->token, API_kickChatMember, chat_id,
        user_id, until_date);

    result = json_is_true(json);

    json_decref(json);

    return result;
}

bool kick_chat_member_chat (Bot *bot, long int chat_id, long int user_id, char *until_date) {
    bool result;
    char * cchat_id;

    cchat_id = api_ltoa(chat_id);

    result = kick_chat_member (bot, cchat_id, user_id, until_date);

    free(cchat_id);

    return result;
}

/**
 * restrictChatMember
 * Use this method to restrict a user in a supergroup. 
 * The bot must be an administrator in the supergroup
 * for this to work and must have the appropriate admin rights. 
 * Pass True for all boolean parameters to lift restrictions from a user. 
 * Returns True on success.
 * https://core.telegram.org/bots/api#restrictchatmember
 */
bool restrict_chat_member (Bot *bot, char *chat_id, long int user_id, long int until_date,
            bool can_send_messages, bool can_send_media_messages, bool can_send_other_messages,
            bool can_add_web_page_previews) {

    int result;
    json_t *json;

    json = generic_method_call(bot->token, API_restrictChatMember,
        chat_id, user_id, until_date,
        (can_send_messages > 0 ? "true" : "0"),
        (can_send_media_messages > 0 ? "true" : "0"),
        (can_send_other_messages > 0 ? "true" : "0"),
        (can_add_web_page_previews > 0 ? "true" : "0") );

    result = json_is_true(json);

    json_decref(json);

    return result;
}


bool restrict_chat_member_chat (Bot *bot, long int chat_id, long int user_id, long int until_date,
            bool can_send_messages, bool can_send_media_messages, bool can_send_other_messages,
            bool can_add_web_page_previews) {
    bool result;
    char * cchat_id;

    cchat_id = api_ltoa(chat_id);

    result = restrict_chat_member (bot, cchat_id, user_id, until_date,
            can_send_messages, can_send_media_messages, can_send_other_messages,
            can_add_web_page_previews);

    free(cchat_id);

    return result;
}



/**
 * unbanChatMember
 * Use this method to unban a previously kicked user in a supergroup or channel. 
 * The user will not return to the group or channel automatically, 
 * but will be able to join via link, etc. 
 * The bot must be an administrator for this to work. 
 * Returns True on success.
 * https://core.telegram.org/bots/api#unbanchatmember
 */
bool unban_chat_member (Bot *bot, char *chat_id, long int user_id) {
    int result;
    json_t *json;

    json = generic_method_call(bot->token, API_unbanChatMember,
        chat_id, user_id);

    result = json_is_true(json);

    json_decref(json);

    return result;
}



bool unban_chat_member_chat (Bot *bot, long int chat_id, long int user_id) {
    bool result;
    char * cchat_id;

    cchat_id = api_ltoa(chat_id);

    result = unban_chat_member (bot, cchat_id, user_id);

    free(cchat_id);

    return result;
}


/**
 * leaveChat
 * Use this method for your bot to leave a group, supergroup or channel. Returns True on success.
 * https://core.telegram.org/bots/api#leavechat
 */
bool leave_chat (Bot *bot, char *chat_id) {
    int result;
    json_t *json;

    json = generic_method_call(bot->token, API_leaveChat, chat_id);
    
    result = json_is_true(json);

    json_decref(json);

    return result;
}



bool leave_chat_chat (Bot *bot, long int chat_id) {
    bool result;
    char * cchat_id;

    cchat_id = api_ltoa(chat_id);

    result = leave_chat (bot, cchat_id);

    free(cchat_id);

    return result;
}




/**
 * promoteChatMember
 * Use this method to promote or demote a user in a supergroup or a channel.
 * The bot must be an administrator in the chat for this to work
 * and must have the appropriate admin rights.
 * Pass False for all boolean parameters to demote a user.
 * Returns True on success.
 * https://core.telegram.org/bots/api#promotechatmember
 */
bool promote_chat_member (Bot *bot, char *chat_id, long int user_id, bool can_change_info,
            bool can_post_messages, bool can_edit_messages, bool can_delete_messages,
            bool can_invite_users, bool can_restrict_members, bool can_pin_messages,
            bool can_promote_members) {
    int result;
    json_t *json;

    json = generic_method_call(bot->token, API_promoteChatMember,
        chat_id, user_id, (can_change_info > 0 ? "true" : "0"),
        (can_post_messages > 0 ? "true": "0"),
        (can_edit_messages > 0 ? "true" : "0"),
        (can_delete_messages > 0 ? "true" : "0"),
        (can_invite_users > 0 ? "true": "0"),
        (can_restrict_members > 0 ? "true" : "0"),
        (can_pin_messages > 0 ? "true" : "0"),
        (can_promote_members > 0 ? "true" : "0"));

    result = json_is_true(json);

    json_decref(json);

    return result;
}


bool promote_chat_member_chat (Bot *bot, long int chat_id, long int user_id, bool can_change_info,
            bool can_post_messages, bool can_edit_messages, bool can_delete_messages,
            bool can_invite_users, bool can_restrict_members, bool can_pin_messages,
            bool can_promote_members) {
    bool result;
    char * cchat_id;

    cchat_id = api_ltoa(chat_id);

    result = promote_chat_member(bot, cchat_id, user_id, can_change_info,
            can_post_messages, can_edit_messages, can_delete_messages,
            can_invite_users, can_restrict_members, can_pin_messages,
            can_promote_members);

    free(cchat_id);

    return result;
}


/**
 * exportChatInviteLink
 * Use this method to export an invite link to a supergroup or a channel.
 * The bot must be an administrator in the chat for this to work
 * and must have the appropriate admin rights.
 * Returns exported invite link as String on success.
 * 
 * You must release the returned string
 * https://core.telegram.org/bots/api#exportchatinvitelink
 */
char *export_chat_invite_link (Bot *bot, char *chat_id) {

    json_t *json;
    char * invite_link;

    json = generic_method_call(bot->token, API_exportChatInviteLink);

    invite_link = alloc_string(json_string_value(json));

    json_decref(json);

    return invite_link;
}


char *export_chat_invite_link_chat (Bot *bot, long int chat_id) {
    char * invite_link, *cchat_id;

    cchat_id = api_ltoa(chat_id);

    invite_link = export_chat_invite_link(bot, cchat_id);

    free(cchat_id);

    return invite_link;
}

/**
 * setChatPhoto
 *
 */
int set_chat_photo(Bot *bot, char * chat_id, char *filename){
    bool result;

    IFile ifile;

    ifile.type = SETCHATPHOTO;
    
    /* Unique identifier for the target */
    ifile.chatphoto.chat_id = chat_id;

    ifile.chatphoto.filename = filename;

    MemStore * input;
    json_t * json;

    input = call_method_input_file(bot->token, ifile);

    json = start_json(input->content);
    mem_store_free(input);

    result = json_is_true(json);

    json_decref(json);

    return result;
}

int set_chat_photo_chat(Bot *bot, long int chat_id, char *filename){
    bool result;
    char * cchat_id;

    cchat_id = api_ltoa(chat_id);

    result = set_chat_photo(bot, cchat_id, filename);

    free(cchat_id);

    return result;
}


/**
 * deleteChatPhoto
 *
 */
int delete_chat_photo(Bot *bot, char *chat_id){
    json_t *json;
    bool btrue;

    json = generic_method_call(bot->token, API_deleteChatPhoto,
        chat_id);

    btrue = json_is_true(json);

    json_decref(json);

    return btrue;
}

int delete_chat_photo_chat(Bot *bot, long int chat_id){
    bool result;
    char * cchat_id;

    cchat_id = api_ltoa(chat_id);

    result = delete_chat_photo(bot, cchat_id);

    free(cchat_id);

    return result;
}

/**
 * getChatAdministrators
 * Use this method to get a list of administrators in a chat.
 * On success, returns an Array of ChatMember objects
 * that contains information about all chat administrators except other bots.
 * If the chat is a group or a supergroup and no administrators were appointed,
 * only the creator will be returned.
 */
ChatMember *get_chat_administrators (Bot *bot, char *chat_id) {
    ChatMember * chat_member_adm;
    json_t *json;

    json = generic_method_call(bot->token, API_getChatAdministrators, chat_id);
    
    chat_member_adm = chat_member_array_parse(json);

    json_decref(json);

    return chat_member_adm;
}

ChatMember *get_chat_administrators_chat(Bot *bot, long int chat_id){
    ChatMember *chat_member_adm;
    char *cchat_id;

    cchat_id = api_ltoa(chat_id);

    chat_member_adm = get_chat_administrators(bot, cchat_id);

    free(cchat_id);

    return chat_member_adm;
}

/**
 * https://core.telegram.org/bots/api#pinchatmessage
 */
bool pin_chat_message (Bot *bot, char *chat_id, long int message_id, bool disable_notification) {
    int result;
    json_t *json;

    json = generic_method_call(bot->token, API_pinChatMessage, chat_id, message_id,
        (disable_notification > 0 ? "true" : "0"));
    
    result = json_is_true(json);

    json_decref(json);

    return result;
}

bool pin_chat_message_chat(Bot *bot, long int chat_id, long int message_id, bool disable_notification) {
    bool result;
    char * cchat_id;

    cchat_id = api_ltoa(chat_id);

    result = pin_chat_message(bot, cchat_id, message_id, disable_notification);

    free(cchat_id);

    return result;
}
/**
 * unpinChatMessage
 *
 */
bool unpin_chat_message(Bot *bot, char *chat_id){
    int result;
    json_t *json;

    json = generic_method_call(bot->token, API_unpinChatMessage,
            chat_id);

    result = json_is_true(json);

    json_decref(json);

    return result;
}

bool unpin_chat_message_chat(Bot *bot, long int chat_id){
    bool result;
    char * cchat_id;

    cchat_id = api_ltoa(chat_id);

    result = unpin_chat_message(bot, cchat_id);

    free(cchat_id);

    return result;
}

/**
 * Generic method to handle Telegram API Methods responses
 * TODO:
 *  - Error filtering
 */
json_t *generic_method_call (const char *token, char *formats, ...) {
    json_t *result;
    va_list params;
    va_start(params, formats);

    char *method_base = vsformat(formats, params);
    MemStore *response = call_method(token, method_base);
    free(method_base);

    if(response){
        result = start_json(response->content);
        mem_store_free(response);

        return result;
    }

    return NULL;
}



/**
 * getFile
 * https://core.telegram.org/bots/api#getfile
 */
char * get_file (Bot * bot, char * dir, const char * file_id){

    json_t *json;
    char *path_file;

    json = generic_method_call(bot->token, API_getfile, file_id);

    File * ofile = file_parse(json);

    json_decref(json);

    if(ofile){
        path_file = call_method_download(bot->token, dir, ofile);
        file_free(ofile);
    }

    return path_file;
}



/**
 * getUserProfilePhotos
 * https://core.telegram.org/bots/api#getuserprofilephotos
 */
UserProfilePhotos * get_user_profile_photos(Bot * bot, char * dir, long user_id,
            long offset, long limit){
    UserProfilePhotos * oupp;
    json_t * json;

    json = generic_method_call(bot->token, API_getUserProfilePhotos,
        user_id, offset, limit);

    oupp = user_profile_photos_parse(json);

    json_decref(json);

    return oupp;
}


/**
 * sendPhoto
 *
 */
Message * send_photo(Bot * bot, char * chat_id, char * filename,
                             char * caption, bool disable_notification,
                             long int reply_to_message_id, char * reply_markup){
    Message * message;

    IFile ifile;

    ifile.type = SENDPHOTO;
    
    /* Unique identifier for the target */
    ifile.photo.chat_id = chat_id;

    /* Photo to send */
    ifile.photo.filename = filename;

    /* Photo caption (may also be used when resending 
     * photos by file_id), 0-200 characters */
    ifile.photo.caption = caption;

    /* Sends the message silently */
    ifile.photo.disable_notification = (disable_notification > 0 ? "true" : NULL);

    /* If the message is a reply, ID of the original message */
    ifile.photo.reply_to_message_id = reply_to_message_id > 0 ? api_ltoa(reply_to_message_id) : NULL;

    ifile.photo.reply_markup = reply_markup;

    MemStore * input;
    json_t * json;

    input = call_method_input_file(bot->token, ifile);

    json = start_json(input->content);

    free(ifile.photo.reply_to_message_id);
    mem_store_free(input);


    message = message_parse(json);

    json_decref(json);

    return message;
}



Message * send_photo_chat(Bot * bot, long int chat_id, char * filename, char * caption,
            bool disable_notification, long int reply_to_message_id, char * reply_markup){

    Message * message;
    char * cchat_id;
    
    cchat_id = api_ltoa(chat_id);

    message = send_photo(bot, cchat_id, filename, caption,
                                 disable_notification, reply_to_message_id,
                                 reply_markup);

    free(cchat_id);

    return message;
}


/**
 * sendAudio
 *
 */
Message * send_audio(Bot *bot, char * chat_id, char * filename, char * caption,
            long int duration, char * performer, char * title, bool disable_notification,
            long int reply_to_message_id, char * reply_markup){
    
    Message * message;

    IFile ifile;

    ifile.type = SENDAUDIO;

    /* Unique identifier for the target */
    ifile.audio.chat_id = chat_id;

    /* Audio file to send */
    ifile.audio.filename = filename;

    /* Audio caption, 0-200 characters */
    ifile.audio.caption = caption;

    /* Duration of the audio in seconds */
    ifile.audio.duration = duration > 0 ? api_ltoa(duration) : NULL;


    /* Performer */
    ifile.audio.performer = performer;

    /* Track name */
    ifile.audio.title = title;

    /* Sends the message silently */
    ifile.audio.disable_notification = (disable_notification > 0 ? "true" : NULL);

    /* If the message is a reply, ID of the original message */
    ifile.audio.reply_to_message_id = reply_to_message_id > 0 ? api_ltoa(reply_to_message_id) : NULL;

    ifile.audio.reply_markup = reply_markup;

    MemStore * input;
    json_t * json;

    input = call_method_input_file(bot->token, ifile);

    json = start_json(input->content);

    free(ifile.audio.duration);
    free(ifile.audio.reply_to_message_id);
    mem_store_free(input);


    message = message_parse(json);

    json_decref(json);

    return message;
}



Message * send_audio_chat(Bot * bot, long int chat_id, char * filename, char * caption,
            long int duration, char * performer, char * title, bool disable_notification,
            long int reply_to_message_id, char * reply_markup){

    Message * message;
    char * cchat_id;
    
    cchat_id = api_ltoa(chat_id);

    message = send_audio(bot, cchat_id, filename, caption, duration,
                                 performer, title, disable_notification,
                                 reply_to_message_id, reply_markup);

    free(cchat_id);

    return message;
}


/**
 * sendDocument
 *
 */
Message * send_document(Bot * bot, char * chat_id, char * filename, char * caption,
            bool disable_notification, long int reply_to_message_id, char * reply_markup){

    Message * message;

    IFile ifile;

    ifile.type = SENDDOCUMENT;

    /* Unique identifier for the target */
    ifile.document.chat_id = chat_id;

    /* Document to send */
    ifile.document.filename = filename;

    /* Document caption (may also be used when resending 
     * Documents by file_id), 0-200 characters */
    ifile.document.caption = caption;

    /* Sends the message silently */
    ifile.document.disable_notification = (disable_notification > 0 ? "true" : NULL);

    /* If the message is a reply, ID of the original message */
    ifile.document.reply_to_message_id = reply_to_message_id > 0 ? api_ltoa(reply_to_message_id) : NULL;

    ifile.document.reply_markup = reply_markup;

    MemStore * input;
    json_t * json;

    input = call_method_input_file(bot->token, ifile);

    json = start_json(input->content);

    free(ifile.document.reply_to_message_id);
    mem_store_free(input);

    message = message_parse(json);

    return message;
}




Message * send_document_chat(Bot * bot, long int chat_id, char * filename, char * caption,
            bool disable_notification, long int reply_to_message_id, char * reply_markup){

    Message * message;
    char * cchat_id;
    
    cchat_id = api_ltoa(chat_id);

    message = send_document(bot, cchat_id, filename, caption,
                                 disable_notification, reply_to_message_id,
                                 reply_markup);

    free(cchat_id);

    return message;
}


/**
 * sendVideo
 *
 */
Message * send_video(Bot * bot, char * chat_id, char * filename, long int duration,
            long int width, long int height, char * caption, bool disable_notification,
            long int reply_to_message_id, char * reply_markup){
    Message * message;

    IFile ifile;

    ifile.type = SENDVIDEO;

    /* Unique identifier for the target */
    ifile.video.chat_id = chat_id;

    /* Audio file to send */
    ifile.video.filename = filename;

    /* Duration of the audio in seconds */
    ifile.video.duration = duration > 0 ? api_ltoa(duration) : NULL;

    /* Video width */
    ifile.video.width = width > 0 ? api_ltoa(width) : NULL;

    /* Video height */
    ifile.video.height = height > 0 ? api_ltoa(height) : NULL;

    /* Audio caption, 0-200 characters */
    ifile.video.caption = caption;

    /* Sends the message silently */
    ifile.video.disable_notification = (disable_notification > 0 ? "true" : NULL);

    /* If the message is a reply, ID of the original message */
    ifile.video.reply_to_message_id = reply_to_message_id > 0 ? api_ltoa(reply_to_message_id) : NULL;

    ifile.video.reply_markup = reply_markup;

    MemStore * input;
    json_t * json;

    input = call_method_input_file(bot->token, ifile);

    json = start_json(input->content);

    free(ifile.video.duration);
    free(ifile.video.width);
    free(ifile.video.height);
    free(ifile.video.reply_to_message_id);
    mem_store_free(input);


    message = message_parse(json);

    json_decref(json);

    return message;
}



Message * send_video_chat(Bot * bot, long int chat_id, char * filename, long int duration,
            long int width, long int height, char * caption, bool disable_notification,
            long int reply_to_message_id, char * reply_markup){

    Message * message;
    char * cchat_id;
    
    cchat_id = api_ltoa(chat_id);

    message = send_video(bot, cchat_id, filename, duration, width, height, caption,
                disable_notification, reply_to_message_id, reply_markup);

    free(cchat_id);

    return message;
}


/**
 * sendVoice
 *
 */
Message * send_voice(Bot *bot, char * chat_id, char * filename, char * caption,
            long int duration, bool disable_notification, long int reply_to_message_id,
            char * reply_markup){
    Message * message;

    IFile ifile;

    ifile.type = SENDVOICE;

    /* Unique identifier for the target */
    ifile.voice.chat_id = chat_id;

    /* Audio file to send */
    ifile.voice.filename = filename;

    /* Audio caption, 0-200 characters */
    ifile.voice.caption = caption;

    /* Duration of the audio in seconds */
    ifile.voice.duration = duration > 0 ? api_ltoa(duration) : NULL;

    /* Sends the message silently */
    ifile.voice.disable_notification = (disable_notification > 0 ? "true" : NULL);

    /* If the message is a reply, ID of the original message */
    ifile.voice.reply_to_message_id = reply_to_message_id > 0 ? api_ltoa(reply_to_message_id) : NULL;

    ifile.voice.reply_markup = reply_markup;

    MemStore * input;
    json_t * json;

    input = call_method_input_file(bot->token, ifile);

    json = start_json(input->content);

    free(ifile.voice.duration);
    free(ifile.voice.reply_to_message_id);
    mem_store_free(input);


    message = message_parse(json);

    json_decref(json);

    return message;
}



Message * send_voice_chat(Bot *bot, long int chat_id, char * filename, char * caption, long int duration,
            bool disable_notification, long int reply_to_message_id, char * reply_markup){

    Message * message;
    char * cchat_id;
    
    cchat_id = api_ltoa(chat_id);

    message = send_voice(bot, cchat_id, filename, caption, duration,
                                 disable_notification,reply_to_message_id,
                                 reply_markup);

    free(cchat_id);

    return message;
}


/**
 * sendVideoNote
 *
 */
Message * send_video_note(Bot * bot, char * chat_id, char * filename, long int duration,
            long int length, bool disable_notification, long int reply_to_message_id, char * reply_markup){
    Message * message;

    IFile ifile;

    ifile.type = SENDVIDEONOTE;

    /* Unique identifier for the target */
    ifile.videonote.chat_id = chat_id;

    /* Document to send */
    ifile.videonote.filename = filename;

    /* Duration of the audio in seconds */
    ifile.videonote.duration = duration > 0 ? api_ltoa(duration) : NULL;

    /* Video width and height */
    ifile.videonote.length = length > 0 ? api_ltoa(length) : NULL;

    /* Sends the message silently */
    ifile.videonote.disable_notification = disable_notification > 0 ? "true" : NULL;

    /* If the message is a reply, ID of the original message */
    ifile.videonote.reply_to_message_id = (reply_to_message_id > 0 ? api_ltoa(reply_to_message_id) : NULL);

    ifile.videonote.reply_markup = reply_markup;

    MemStore * input;
    json_t * json;

    input = call_method_input_file(bot->token, ifile);

    json = start_json(input->content);

    free(ifile.videonote.duration);
    free(ifile.videonote.length);
    free(ifile.videonote.reply_to_message_id);
    mem_store_free(input);

    message = message_parse(json);

    json_decref(json);

    return message;
}



Message * send_video_note_chat(Bot * bot, long int chat_id, char * filename, long int duration,
            long int length, bool disable_notification, long int reply_to_message_id, char * reply_markup){

    Message * message;
    char * cchat_id;
    
    cchat_id = api_ltoa(chat_id);

    message = send_video_note(bot, cchat_id, filename, duration, length, disable_notification,
                reply_to_message_id, reply_markup);

    free(cchat_id);

    return message;
}



Error * show_error(){
    Error * error = get_error();

    return error;
}


/**
 * forwardMessage
 *
 */
Message * forward_message_from (Bot * bot, long int chat_id, char * from_chat_id,
            bool disable_notification, long int message_id){

    Message * message;
    char * cchat_id;

    cchat_id = api_ltoa(chat_id);

    message =  forward_message(bot, cchat_id, from_chat_id, disable_notification, message_id);

    free(cchat_id);

    return message;
}



Message * forward_message_from_chat (Bot * bot, char * chat_id, long int from_chat_id, 
            bool disable_notification, long int message_id){

    Message * message;
    char * cfrom_chat_id;

    cfrom_chat_id = api_ltoa(from_chat_id);

    message =  forward_message(bot, chat_id, cfrom_chat_id, disable_notification, message_id);

    free(cfrom_chat_id);

    return message;
}



Message * forward_message (Bot * bot, char * chat_id, char * from_chat_id, 
            bool disable_notification, long int message_id){
    Message * message;
    json_t *json;

    json = generic_method_call(bot->token, API_forwardMessage, chat_id, from_chat_id,
        (disable_notification > 0 ? "true" : "0"), message_id);

    message = message_parse(json);

    json_decref(json);

    return message;
}



Message * forward_message_chat (Bot * bot, long int chat_id, long int from_chat_id, 
            bool disable_notification, long int message_id){

    Message * message;
    char * cchat_id, *cfrom_chat_id;

    cchat_id = api_ltoa(chat_id);

    cfrom_chat_id = api_ltoa(from_chat_id);

    message =  forward_message(bot, cchat_id, cfrom_chat_id, disable_notification, message_id);

    free(cchat_id);
    free(cfrom_chat_id);

    return message;
}


/**
 * sendLocation
 *
 */
Message * send_location (Bot * bot, char * chat_id, float latitude,
            float longitude, long int live_period, bool disable_notification,
            long int reply_to_message_id, char * reply_markup){
    Message * message;
    json_t * json;

    json = generic_method_call(bot->token, API_sendLocation,
        chat_id, latitude, longitude, live_period,
        (disable_notification > 0 ? "true" : 0),
        reply_to_message_id, reply_markup);

    message = message_parse(json);

    json_decref(json);

    return message;
}



Message * send_location_chat (Bot * bot, long int chat_id, float latitude, float logitude,
            long int live_period, bool disable_notification, long int reply_to_message_id,
            char * reply_markup){

    Message * message;
    char * cchat_id;

    cchat_id = api_ltoa(chat_id);

    message = send_location(bot, cchat_id, latitude, logitude, live_period,
                disable_notification, reply_to_message_id, reply_markup);

    free(cchat_id);

    return message;
}


/**
 * sendContact
 *
 */
Message * send_contact(Bot * bot, char * chat_id, char * phone_number, char * first_name,
            char * last_name, bool disable_notification, long int reply_to_message_id,
            char * reply_markup){
    json_t * json;
    Message * message;

    json = generic_method_call(bot->token, API_sendContact,
        chat_id, phone_number, first_name, last_name,
        (disable_notification > 0 ? "true":"0"),
        reply_to_message_id, reply_markup);

    message = message_parse(json);

    json_decref(json);

    return message;
}



Message * send_contact_chat(Bot * bot, long int chat_id, char * phone_number, char * first_name,
            char * last_name, bool disable_notification, long int reply_to_message_id, char * reply_markup){
    Message * message;
    char * cchat_id;

    cchat_id = api_ltoa(chat_id);

    message = send_contact(bot, cchat_id, phone_number, first_name, last_name,
        disable_notification, reply_to_message_id, reply_markup);

    free(cchat_id);

    return message;
}


/**
 * sendChatAction
 *
 */
int send_chat_action(Bot * bot, char * chat_id, char * action){
    json_t * json;
    int result;

    json = generic_method_call(bot->token, API_sendChatAction,
            chat_id, action);

    result = json_is_true(json) ? 0 : -1;

    return result;
}

int send_chat_action_chat(Bot * bot, long int chat_id, char * action){
    int result;
    char * cchat_id;

    cchat_id = api_ltoa(chat_id);

    result = send_chat_action(bot, cchat_id, action);

    free(cchat_id);

    return result;
}


/**
 * sendVenue
 *
 */
Message * send_venue(Bot * bot, char * chat_id, float latitude, float longitude,
            char * title, char * address, char * foursquare_id, bool disable_notification,
            long int reply_to_message_id, char * reply_markup){
    json_t * json;
    Message * message;

    json = generic_method_call(bot->token, API_sendVenue,
        chat_id, latitude, longitude, title, address, foursquare_id,
        (disable_notification > 0 ? "true" : "0"),
        reply_to_message_id, reply_markup);

    message = message_parse(json);

    json_decref(json);

    return message;
}



Message * send_venue_chat(Bot * bot, long int chat_id, float latitude, float longitude,
            char * title, char * address, char * foursquare_id, bool disable_notification,
            long int reply_to_message_id, char * reply_markup){
    Message * message;
    char * cchat_id;

    cchat_id = api_ltoa(chat_id);

    message = send_venue(bot, cchat_id, latitude, longitude, title,
        address, foursquare_id, disable_notification, reply_to_message_id,
        reply_markup);

    free(cchat_id);

    return message;
}


/**
 * editMessageLiveLocation
 *
 */
Message * edit_message_live_location(Bot * bot, char * chat_id, long int message_id,
            char * inline_message_id, float latitude, float longitude, char * reply_markup){
    Message * message;
    json_t * json;

    json = generic_method_call(bot->token, API_editMessageLiveLocation,
        chat_id, message_id, inline_message_id, latitude, longitude, reply_markup);

    message = message_parse(json);

    json_decref(json);

    return message;
}

Message * edit_message_live_location_chat(Bot * bot, long int chat_id, long int message_id,
            char * inline_message_id, float latitude, float longitude, char * reply_markup){
    Message * message;
    char * cchat_id;

    cchat_id = api_ltoa(chat_id);

    message = edit_message_live_location(bot, cchat_id, message_id, inline_message_id,
        latitude, longitude, reply_markup);

    free(cchat_id);

    return message;
}

/**
 * stopMessageLiveLocation
 *
 */
Message * stop_message_live_location(Bot * bot, char * chat_id, long int message_id,
            char * inline_message_id, char * reply_markup){
    json_t * json;
    Message * message;

    json = generic_method_call(bot->token, API_stopMessageLiveLocation,
                chat_id, message_id, inline_message_id,
                reply_markup);

    message = message_parse(json);

    json_decref(json);

    return message;
}

Message * stop_message_live_location_chat(Bot * bot, long int chat_id, long int message_id,
            char * inline_message_id, char * reply_markup){
    Message * message;
    char * cchat_id;

    cchat_id = api_ltoa(chat_id);

    message = stop_message_live_location(bot, cchat_id, message_id,
        inline_message_id, reply_markup);

    free(cchat_id);

    return message;
}

/**
 * editMessageText
 *
 */
Message *edit_message_text(Bot *bot, char *chat_id, long int message_id,
    char *inline_message_id, char *text, char *parse_mode,
    bool disable_web_page_preview, char *reply_markup){
    Message *message;
    json_t *json;

    json = generic_method_call(bot->token, API_editMessageText, chat_id, message_id,
        inline_message_id, text, parse_mode, disable_web_page_preview, reply_markup);

    message = message_parse(json);

    json_decref(json);

    return message;
}

Message *edit_message_text_chat(Bot *bot, long int chat_id, long int message_id,
    char *inline_message_id, char *text, char *parse_mode,
    bool disable_web_page_preview, char *reply_markup){
    Message *message;
    char *cchat_id;

    cchat_id = api_ltoa(chat_id);

    message = edit_message_text(bot, cchat_id, message_id,
        inline_message_id, text, parse_mode, disable_web_page_preview,
        reply_markup);

    free(cchat_id);

    return message;
}

/**
 * editMessageCaption
 *
 */
Message *edit_message_caption(Bot *bot, char *chat_id,
    long int message_id, char *inline_message_id, char *caption,
    char *reply_markup){
    Message *message;
    json_t *json;

    json = generic_method_call(bot->token, API_editMessageCaption, chat_id,
        message_id, inline_message_id, caption, reply_markup);

    message = message_parse(json);

    json_decref(json);

    return message;
}

Message *edit_message_caption_chat(Bot *bot, long int chat_id,
    long int message_id, char *inline_message_id, char *caption,
    char *reply_markup){
    Message *message;
    char *cchat_id;

    cchat_id = api_ltoa(chat_id);

    message = edit_message_caption(bot, cchat_id, message_id,
        inline_message_id, caption, reply_markup);

    free(cchat_id);

    return message;
}


/**
 * editMessageReplyMarkup
 *
 */
Message *edit_message_reply_markup(Bot *bot, char *chat_id, long int message_id,
        char *inline_message_id, char *reply_markup){
    Message *message;
    json_t *json;

    json = generic_method_call(bot->token, API_editMessageReplyMarkup, chat_id,
        message_id, inline_message_id, reply_markup);

    message = message_parse(json);

    json_decref(json);

    return message;
}

Message *edit_message_reply_markup_chat(Bot *bot, long int chat_id, long int message_id,
        char *inline_message_id, char *reply_markup){
    Message *message;
    char *cchat_id;

    cchat_id = api_ltoa(chat_id);

    message = edit_message_reply_markup(bot, cchat_id, message_id,
        inline_message_id, reply_markup);

    free(cchat_id);

    return message;
}

/**
 * deleteMessage
 *
 */
bool delete_message(Bot *bot, char *chat_id, long int message_id){
    bool result;
    json_t *json;

    json = generic_method_call(bot->token, API_deleteMessage,
        chat_id, message_id);

    result = json_is_true(json);

    json_decref(json);

    return result;
}

bool delete_message_chat(Bot *bot, long int chat_id, long int message_id){
    bool result;
    char *cchat_id;

    cchat_id = api_ltoa(chat_id);

    result = delete_message(bot, cchat_id, message_id);

    free(cchat_id);

    return result;
}

/**
 * setChatStickerSet
 *
 */
bool set_chat_sticker_set(Bot *bot, char *chat_id, long int sticker_set_name){
    bool result;
    json_t *json;

    json = generic_method_call(bot->token, API_setChatStickerSet,
        chat_id, sticker_set_name);

    result = json_is_true(json);

    json_decref(json);

    return result;
}

bool set_chat_sticker_set_chat(Bot *bot, long int chat_id, long int sticker_set_name){
    bool result;
    char *cchat_id;

    cchat_id = api_ltoa(chat_id);

    result = set_chat_sticker_set(bot, cchat_id, sticker_set_name);

    free(cchat_id);

    return result;
}

/**
 * deleteChatStickerSet
 *
 */
bool delete_chat_sticker_set(Bot *bot, char *chat_id){
    bool result;
    json_t *json;

    json = generic_method_call(bot->token, API_deleteChatStickerSet, chat_id);

    result = json_is_true(json);

    json_decref(json);

    return result;
}

bool delete_chat_sticker_set_chat(Bot *bot, long int chat_id){
    bool result;
    char *cchat_id;

    cchat_id = api_ltoa(chat_id);

    result = delete_chat_sticker_set(bot, cchat_id);

    free(cchat_id);

    return result;
}
