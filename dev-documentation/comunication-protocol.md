# comunication protocol

```
                  R-TYPE COMMUNICATION PROTOCOL
```

Abstract

This document provides an in depth specification of the client <-> server communication protocol implemented during the Epitech R-Type project. The protocol as described in this document contains all necessary commands a client must implement in order to participate in games run by the R-Type server.

Status of this Memo

This memo is the specification of the Communication Protocol implemented by its authors for the R-Type Epitech project. This specification is an exercise in the writing of an rfc document as defined by RFC 7322 "RFC Style Guide". This specification does not aim to be submitted at any point in time.

Copyright Notice

This document is not subject to any copyright notice and thus expresses no rights reserved.

Table of Contents

```
1. Introduction ...................................................X
2. TCP Protocol ...................................................X
    2.1. Command Syntax ...........................................X
    2.2. Command Response .........................................X
        2.2.1. Response Codes and Meaning .........................X
    2.3. Server Commands ..........................................X
    2.4. Client Commands ..........................................X
3. UDP Protocol ...................................................X
    3.1. Command Syntax ...........................................X
    3.2. Server Commands ..........................................X
        3.2.1. Server Command Listing .............................X
        3.2.2. Server Command Arguments ...........................X
    3.3. Client Commands ..........................................X
        3.3.1. Server Response ....................................X
        3.3.2. Client Command Listing .............................X
        3.3.3. Client Command Arguments ...........................X
```

1.  Introduction

    The R-Type project is an epitech project that aims to produce a multiplayer game based on the R-Type game.

    This project aims to introduce students to the intricate details of building a networked multiplayer game while also focusing on the package the game is delivered in. This includes but is not limited to :

    ```
     - Crossplatform build system
     - Limited user setup required
     - Multithreaded server
     - Inability to cheat using modified client
    ```
2.  TCP Protocol

    The TCP protocol will serve as the main protocol used by the client and server before the start of a game. It will also serve the purpose of handling anything non relevent to the game currently being played. The sole exception to that last bit will be the handling of in game messaging and server message broadcasting.

    2.1. Command Syntax

    ```
     TCP commands follow the following syntax :

             <CMD> <SP> <HEADER>;<ARGS> <CRLF>

     Here is a definition of each part :

     <CMD>    -> This is the command name and will always be fully
                 capitalized.

     <SP>     -> Ascii space character

     <HEADER> -> For client commands, user UUID. For server commands,
                 server UUID. Both distributed by server on
                 connection.
     
     <ARGS>   -> Arguments for given command. These args are
                 separated by the ',' ascii character.

     <CRLF>   -> Carriage Return Line Feed character. Every message
                 will end in this character combination denoted
                 '\r\n' in ascii.
    ```

    2.2. Command Response

    ```
     All client sent commands will generate a response from the
     server allowing the client to know the status of their command.

     Responses will follow the following syntax :

             <CODE> <SP> <HEADER> <SP> <BODY> <CRLF>

     In this format, the HEADER is the server's UUID, that the client
     can use in order to make sure he is interacting with the correct
     server.

     The response format is similar to the command synthaxe, except
     that the cmd is replaced by a three digit response code that
     mimics that of the HTTP protocol.

     2.2.1. Response Codes and Meanings

         The following is a list of all possible response code. This
         list of response codes is strongly inspired by the response
         codes of the HTTP protocol.

             - 500 -> Wrong Request
             - 401 -> Forbidden
             - 200 -> OK
    ```

    2.3. Server Commands

    ```
     The only server command that currently needs any handling by the
     client is the START command.

     The START command will provide one argument, which is the port
     that the client should send his first UDP message to.
    ```

    2.4. Client Commands

    ```
     The following is a list of Client Commands :

         - CONNECT -> Only command with no header. This command
                      allows the client to retrieve his user assigned
                      uuid.

         - START   -> This command allows the user to signal to the
                      server that the game should start.
    ```
3.  UDP Protocol

    The UDP protocol will be the main protocol used to communicated the behaviour of the game to clients during any given game.

    Since in this project the server is supposed to handle most of the game logic this protocol will be centered around the ECS and communicating it's state over the course of a given game.

    NO UDP SERVER COMMAND will await any type of response.

    3.1. Command Synthax

    ```
     Commands follow the following syntax :

             <CMD> <SP> <ARGS> <CRLF>
     
     Here is a definition of each part :

     <CMD>  -> This is the command name and will always be fully
               capitalized.

     <SP>   -> Ascii space character

     <ARGS> -> These are the arguments for the command. The size of
               these args may vary even within the same command. For
               this purpose each argument will be separated by the
               ';' character.
               
               Any sub-argument within each argument will be
               separated by the ',' character.

     <CRLF> -> Carriage Return Line Feed character. Every message
               will end in this character combination denoted '\r\n'
               in ascii.
     
     Here is an example of a valid server command :

                          DEL_ENT 1 \r\n
     
    ```

    3.2. Server Commands

    ```
     This section details the commands sent by the server to clients
     along with their usage.

     3.2.1 Server Command Listing

         The following is a listing of all valid server commands :

                 ENTITY        ->    Provide entity description, with
                                     component args.
                 DEL_ENT       ->    Delete entity
                 DEL_COMP      ->    Delete component
                 CHANGE_MUSIC  ->    Change Song
                 GAME_END      ->    Signals end of game

     3.2.2. Server Command Arguments

         The following is an explanation of the arguments needed for
         each server command :

                   <THIS LIST NEEDS ELABORATING>

         ENTITY ->
             (entity id, (component id, component args...)...)
         DEL_ENT -> (entity id)
         DEL_COMP -> (entity id, component id)
         MUSIC -> (song id)
    ```

    3.3. Client Commands

    ```
     This section details the commands sent by the client to the
     server along with their arguments and usage.

     In contrast with Server Commands, some of these commands will
     elicit a response.

     3.3.1. Server Response

         The server response to client commands will always be in the
         form of server commands. This means and allows the client to
         not have to wait on the server response as the "server
         response" only needs to be handled like any other command.

     3.3.2. Client Command Listing

         These commands WILL illicit a response, this response will
         take the form of commands, so should not be waited for:

             HERE        ->      Signal connection to server
             GET_ENT     ->      Get specification for entity

         These commands WILL NOT illicit any response:

             ACT_SHOOT   ->      Signal the server that the client
                                 shot.
             ACT_MOVE    ->      Signal the server that the client
                                 has moved.
             PING        ->      Signal server that client is
                                 still connected. Required every
                                 three seconds.

     3.3.3. Client Command Arguments

         The following is a listing of the arguments for each client
         command.

             GET_ENT     ->      (entity id)
             ACT_SHOOT   ->      No arguments
             ACT_MOVE    ->      (direction)
             HERE        ->      (client UUID)
    ```
