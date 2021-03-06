# Framebot - Alpha [![Build Status](https://travis-ci.org/giancarlopro/Telebot-TelegramBotAPI.svg?branch=master)](https://travis-ci.org/giancarlopro/Telebot-TelegramBotAPI)

This is the official repository for Framebot.

## Building from source

First you need to install the dependencies

```
apt install jansson openssl libcurl4-openssl-dev
```

then you use cmake to create the build files
```
mkdir build
cd build
cmake ..
```

and use make to build
```
make
```

### Dependencies
* [jansson](https://github.com/akheron/jansson)
* [OpenSSL](https://github.com/openssl/openssl)
* [libcurl](https://curl.haxx.se/libcurl/)

### Methods
- [ ] webhooks
- [x] getupdates
- [x] getMe
- [x] forwardMessage
- [x] sendPhoto
- [x] sendAudio
- [x] sendDocument
- [x] sendVideo
- [x] sendVoice
- [x] sendVideoNote
- [ ] sendMediaGroup
- [x] sendLocation
- [x] editMessageLiveLocation
- [x] stopMessageLiveLocation
- [x] sendVenue
- [x] sendContact
- [x] sendChatAction
- [x] getUserProfilePhotos
- [x] getFile
- [x] kickChatMember
- [x] unbanChatMember
- [x] restrictChatMember
- [x] promoteChatMember
- [x] exportChatInviteLink
- [x] setChatPhoto
- [x] deleteChatPhoto
- [x] setChatDescription
- [x] setChatTitle
- [x] pinChatMessage
- [x] unpinChatMessage
- [x] leaveChat
- [x] getChat
- [x] getChatAdministrators
- [x] getChatMembersCount
- [x] getChatMember
- [x] setChatStickerSet
- [x] deleteChatStickerSet
- [ ] answerCallbackQuery

### Updating messages
- [x] editMessageText
- [x] editMessageCaption
- [x] editMessageReplyMarkup
- [x] deleteMessage
