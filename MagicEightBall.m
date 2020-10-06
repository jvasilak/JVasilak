%% MagicEightBall.m
% This program will simulate the use of magic eightball, where you ask a
% yes or no question and the eight ball randomly gives you a response,
% using a structured array to store answers and then randomly gives one of
% these answers when the user asks a question.

% Jonathon Vasilak
% April 8, 2020

clear
clc

%% Generate Responses
% Uses a structured array to create ten responses to the user's question

eightball(1).response = 'It is certain';
eightball(2).response = 'My sources say no';
eightball(3).response = 'Outlook doesn''t look good';
eightball(4).response = 'Most likely';
eightball(5).response = 'Concentrate and ask again';
eightball(6).response = 'As I see it, yes';
eightball(7).response = 'Cannot predict now';
eightball(8).response = 'My reply is no';
eightball(9).response = 'Very doubtful';
eightball(10).response = 'Yes';

%% Asks user question and generates random answer
% Uses display function to prompt the user to ask a question,
% stores the question as a variable, and gives an answer to the user that
% is randomly determined

disp('I am the magic eightball, ask me a yes or no question:')
user_response = input('Question: ','s');

response_number = randi(10);

disp(eightball(response_number).response)