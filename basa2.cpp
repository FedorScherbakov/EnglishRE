#include <iostream>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QCoreApplication>
#include <QDebug>
#include <vector>
#include <tuple>

void checkSqlError(const QSqlError& error, const QString& message) {
    if (error.type() != QSqlError::NoError) {
        qDebug() << "Ошибка:" << message;
        qDebug() << "Driver Text:" << error.driverText();
        qDebug() << "Database Text:" << error.databaseText();
        QCoreApplication::exit(1);
    }
}

void createDatabase(const QString& dbName = "english_test3.db") {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);

    if (!db.open()) {
        qDebug() << "Не удалось открыть базу данных:" << db.lastError().text();
        QCoreApplication::exit(1);
        return;
    }

    QSqlQuery query(db);

    QString createGrammarTableSql = R"(
        CREATE TABLE IF NOT EXISTS grammar_questions (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            level TEXT NOT NULL,
            question TEXT NOT NULL,
            answer TEXT NOT NULL
        )
    )";

    QString createTranslationTableSql = R"(
        CREATE TABLE IF NOT EXISTS translation_questions (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            level TEXT NOT NULL,
            question_ru TEXT NOT NULL,
            answer_en TEXT NOT NULL
        )
    )";

    QString createExpressionTableSql = R"(
        CREATE TABLE IF NOT EXISTS expression_questions (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            level TEXT NOT NULL,
            prompt TEXT NOT NULL,
            correct_answer TEXT NOT NULL
        )
    )";

    if (!query.exec(createGrammarTableSql)) {
        qDebug() << "Не удалось создать таблицу grammar_questions:" << query.lastError().text();
        db.close();
        QCoreApplication::exit(1);
        return;
    }

    if (!query.exec(createTranslationTableSql)) {
        qDebug() << "Не удалось создать таблицу translation_questions:" << query.lastError().text();
        db.close();
        QCoreApplication::exit(1);
        return;
    }

    if (!query.exec(createExpressionTableSql)) {
        qDebug() << "Не удалось создать таблицу expression_questions:" << query.lastError().text();
        db.close();
        QCoreApplication::exit(1);
        return;
    }

    qDebug() << "База данных '" << dbName << "' успешно создана.";
    db.close();
}

void populateGrammarQuestions(const QString& dbName = "english_test3.db") {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);

    if (!db.open()) {
        qDebug() << "Не удалось открыть базу данных:" << db.lastError().text();
        QCoreApplication::exit(1);
        return;
    }

    QSqlQuery query(db);

    std::vector<std::tuple<QString, QString, QString>> questions = {
        {"A0", "What is this?", "This is a pen."},
        {"A1", "She ___ a book now.", "is reading"},
        {"A2", "If I had money, I ___ a new car.", "would buy"},
        {"B1", "Choose the correct option:  Despite / Although the rain, we went out.", "Despite"},
        {"B2", "Rewrite the sentence using a passive voice: They have built a new bridge.", "A new bridge has been built."},
        {"C1", "Which sentence uses the correct punctuation? A, B, or C? A. However, I think; B. However, I think, C. However, I think.", "C. However, I think."},
        {"A0", "What ___ this?", "is"},
        {"A0", "___ you a student?", "Are"},
        {"A0", "This is ___ apple.", "an"},
        {"A0", "I ___ from Russia.", "am"},
        {"A0", "He ___ a doctor.", "is"},
        {"A0", "This is ___ book.", "a"},
        {"A0", "They ___ students.", "are"},
        {"A0", "She ___ a teacher.", "is"},
        {"A0", "It ___ a cat.", "is"},
        {"A0", "You ___ a friend.", "are"},
        {"A0", "___ is a book.", "This"},
        {"A0", "___ am a student.", "I"},
        {"A0", "___ are my friends.", "They"},
        {"A0", "It ___ a dog.", "is"},
        {"A0", "You ___ happy.", "are"},
        {"A0", "He ___ tall.", "is"},
        {"A0", "She ___ a doctor.", "is"},
        {"A0", "We ___ from Spain.", "are"},
        {"A0", "___ is my pen.", "This"},
        {"A0", "___ are my shoes.", "These"},

        {"A1", "She ___ English very well.", "speaks"},
        {"A1", "They ___ to the cinema every week.", "go"},
        {"A1", "I ___ a dog.", "have"},
        {"A1", "He ___ like coffee.", "doesn't"},
        {"A1", "Where ___ you live?", "do"},
        {"A1", "I ___ born in 1990.", "was"},
        {"A1", "What ___ you do yesterday?", "did"},
        {"A1", "He ___ a book at the moment.", "is reading"},
        {"A1", "They ___ arrived yet.", "haven't"},
        {"A1", "There is ___ milk in the fridge.", "some"},
        {"A1", "We ___ to the park last Sunday.", "went"},
        {"A1", "She ___ a new dress.", "bought"},
        {"A1", "He ___ play football very well.", "can"},
        {"A1", "They ___ going to visit us.", "are"},
        {"A1", "This is ___ interesting film.", "an"},
        {"A1", "She ___ English very well.", "speaks"},
        {"A1", "They ___ to the cinema every week.", "go"},
        {"A1", "I ___ a dog.", "have"},
        {"A1", "He ___ like coffee.", "doesn't"},
        {"A1", "Where ___ you live?", "do"},
        {"A1", "I ___ born in 1990.", "was"},
        {"A1", "What ___ you do yesterday?", "did"},
        {"A1", "He ___ a book at the moment.", "is reading"},
        {"A1", "They ___ arrived yet.", "haven't"},
        {"A1", "There is ___ milk in the fridge.", "some"},
        {"A1", "We ___ to the park last Sunday.", "went"},
        {"A1", "She ___ a new dress.", "bought"},
        {"A1", "He ___ play football very well.", "can"},
        {"A1", "They ___ going to visit us.", "are"},
        {"A1", "This is ___ interesting film.", "an"},

        {"A2", "If I ___ you, I would apologize.", "were"},
        {"A2", "He is taller ___ his brother.", "than"},
        {"A2", "She ___ to the party last night.", "went"},
        {"A2", "I ___ been to Paris.", "have never"},
        {"A2", "___ you ever eaten sushi?", "Have"},
        {"A2", "He said that he ___ tired.", "was"},
        {"A2", "They ___ be here by now.", "should"},
        {"A2", "This is the book ___ I told you about.", "that"},
        {"A2", "He is used to ___ up early.", "getting"},
        {"A2", "The film was very ___.", "exciting"},
        {"A2", "I wish I ___ more money.", "had"},
        {"A2", "He ___ be very happy to see you.", "will"},
        {"A2", "She ___ going to the beach tomorrow.", "is"},
        {"A2", "They ___ a good time at the party.", "had"},
        {"A2", "The weather ___ beautiful yesterday.", "was"},
        {"A2", "If I ___ you, I would apologize.", "were"},
        {"A2", "He is taller ___ his brother.", "than"},
        {"A2", "She ___ to the party last night.", "went"},
        {"A2", "I ___ been to Paris.", "have never"},
        {"A2", "___ you ever eaten sushi?", "Have"},
        {"A2", "He said that he ___ tired.", "was"},
        {"A2", "They ___ be here by now.", "should"},
        {"A2", "This is the book ___ I told you about.", "that"},
        {"A2", "He is used to ___ up early.", "getting"},
        {"A2", "The film was very ___.", "exciting"},
        {"A2", "I wish I ___ more money.", "had"},
        {"A2", "He ___ be very happy to see you.", "will"},
        {"A2", "She ___ going to the beach tomorrow.", "is"},
        {"A2", "They ___ a good time at the party.", "had"},
        {"A2", "The weather ___ beautiful yesterday.", "was"},

        {"B1", "Despite ___ rain, we went out.", "the"},
        {"B1", "I wish I ___ more time.", "had"},
        {"B1", "He told me that he ___ going to be late.", "was"},
        {"B1", "The house ___ built in 1900.", "was"},
        {"B1", "If I had known, I ___ have told you.", "would"},
        {"B1", "She is good at ___ languages.", "learning"},
        {"B1", "He is interested ___ history.", "in"},
        {"B1", "This is the best film I ___ ever seen.", "have"},
        {"B1", "I'm looking forward to ___ you.", "seeing"},
        {"B1", "He has a lot of ___ to do.", "work"},
        {"B1", "Although it was raining, ___ we went out.", "nevertheless"},
        {"B1", "She is ___ than I am.", "taller"},
        {"B1", "He is ___ person I have ever met.", "the nicest"},
        {"B1", "They ___ be arriving soon.", "should"},
        {"B1", "I used ___ to the gym.", "to go"},
        {"B1", "Despite ___ rain, we went out.", "the"},
        {"B1", "I wish I ___ more time.", "had"},
        {"B1", "He told me that he ___ going to be late.", "was"},
        {"B1", "The house ___ built in 1900.", "was"},
        {"B1", "If I had known, I ___ have told you.", "would"},
        {"B1", "She is good at ___ languages.", "learning"},
        {"B1", "He is interested ___ history.", "in"},
        {"B1", "This is the best film I ___ ever seen.", "have"},
        {"B1", "I'm looking forward to ___ you.", "seeing"},
        {"B1", "He has a lot of ___ to do.", "work"},
        {"B1", "Although it was raining, ___ we went out.", "nevertheless"},
        {"B1", "She is ___ than I am.", "taller"},
        {"B1", "He is ___ person I have ever met.", "the nicest"},
        {"B1", "They ___ be arriving soon.", "should"},
        {"B1", "I used ___ to the gym.", "to go"},

        {"B2", "Hardly ___ he entered the room when the phone rang.", "had"},
        {"B2", "He is said ___ be a very talented musician.", "to"},
        {"B2", "The more you practice, ___ better you become.", "the"},
        {"B2", "He was accused ___ stealing the money.", "of"},
        {"B2", "He is capable ___ doing anything he sets his mind to.", "of"},
        {"B2", "I object ___ being treated like that.", "to"},
        {"B2", "She insisted ___ paying for dinner.", "on"},
        {"B2", "The meeting was put ___ until next week.", "off"},
        {"B2", "He takes ___ his father.", "after"},
        {"B2", "I'm fed up ___ this.", "with"},
        {"B2", "Not only ___ he rude, but he was also unhelpful.", "was"},
        {"B2", "Were it not for your help, I ___ have succeeded.", "wouldn't"},
        {"B2", "He is a ___ speaker.", "gifted"},
        {"B2", "The play was ___ received.", "well"},
        {"B2", "The company is ___ to expand.", "poised"},
        {"B2", "Hardly ___ he entered the room when the phone rang.", "had"},
        {"B2", "He is said ___ be a very talented musician.", "to"},
        {"B2", "The more you practice, ___ better you become.", "the"},
        {"B2", "He was accused ___ stealing the money.", "of"},
        {"B2", "He is capable ___ doing anything he sets his mind to.", "of"},
        {"B2", "I object ___ being treated like that.", "to"},
        {"B2", "She insisted ___ paying for dinner.", "on"},
        {"B2", "The meeting was put ___ until next week.", "off"},
        {"B2", "He takes ___ his father.", "after"},
        {"B2", "I'm fed up ___ this.", "with"},
        {"B2", "Not only ___ he rude, but he was also unhelpful.", "was"},
        {"B2", "Were it not for your help, I ___ have succeeded.", "wouldn't"},
        {"B2", "He is a ___ speaker.", "gifted"},
        {"B2", "The play was ___ received.", "well"},
        {"B2", "The company is ___ to expand.", "poised"},

        {"C1", "The nuances of the English language are often ___ to understand.", "subtle"},
        {"C1", "He is a man of ___ few words.", "few"},
        {"C1", "The report is ___ to be finished by Friday.", "due"},
        {"C1", "He is always ___ with excuses.", "coming up"},
        {"C1", "She is a ___ professional.", "consummate"},
        {"C1", "The evidence is ___ conclusive.", "far from"},
        {"C1", "He is a ___ advocate of human rights.", "staunch"},
        {"C1", "The situation is ___ critical.", "becoming"},
        {"C1", "He is known for his ___ wit.", "acerbic"},
        {"C1", "The company is ___ for bankruptcy.", "teetering"},
        {"C1", "His arguments were ___, though persuasive.", "specious"},
        {"C1", "The decision was met with ___ criticism.", "widespread"},
        {"C1", "The project is ___ behind schedule.", "significantly"},
        {"C1", "He is an ___ expert in the field.", "acknowledged"},
        {"C1", "She has a ___ grasp of the subject.", "tenuous"},
        {"C1", "The nuances of the English language are often ___ to understand.", "subtle"},
        {"C1", "He is a man of ___ few words.", "few"},
        {"C1", "The report is ___ to be finished by Friday.", "due"},
        {"C1", "He is always ___ with excuses.", "coming up"},
        {"C1", "She is a ___ professional.", "consummate"},
        {"C1", "The evidence is ___ conclusive.", "far from"},
        {"C1", "He is a ___ advocate of human rights.", "staunch"},
        {"C1", "The situation is ___ critical.", "becoming"},
        {"C1", "He is known for his ___ wit.", "acerbic"},
        {"C1", "The company is ___ for bankruptcy.", "teetering"},
        {"C1", "His arguments were ___, though persuasive.", "specious"},
        {"C1", "The decision was met with ___ criticism.", "widespread"},
        {"C1", "The project is ___ behind schedule.", "significantly"},
        {"C1", "He is an ___ expert in the field.", "acknowledged"},
        {"C1", "She has a ___ grasp of the subject.", "tenuous"},
        {"C1", "The success of the venture hinged ___ his expertise.", "on"},
        {"C1", "His behavior was ___ to the accepted norms.", "antithetical"},
        {"C1", "The policy was implemented with the best ___ .", "intentions"},
        {"C1", "The speaker's eloquence was ___ the audience.", "riveting"},
        {"C1", "Despite his efforts, the situation remained ___ .", "intractable"}
    };

    QString insertSql = R"(
        INSERT INTO grammar_questions (level, question, answer)
        VALUES (:level, :question, :answer)
    )";

    query.prepare(insertSql);

    for (const auto& question : questions) {
        query.bindValue(":level", std::get<0>(question));
        query.bindValue(":question", std::get<1>(question));
        query.bindValue(":answer", std::get<2>(question));

        if (!query.exec()) {
            qDebug() << "Ошибка при вставке строки в grammar_questions:" << query.lastError().text();
            db.close();
            QCoreApplication::exit(1);
            return;
        }
    }

    qDebug() << "Таблица grammar_questions успешно заполнена.";
    db.close();
}

void populateTranslationQuestions(const QString& dbName = "english_test3.db") {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);

    if (!db.open()) {
        qDebug() << "Не удалось открыть базу данных:" << db.lastError().text();
        QCoreApplication::exit(1);
        return;
    }

    QSqlQuery query(db);

    std::vector<std::tuple<QString, QString, QString>> questions = {
        {"A0", "Привет", "Hello"},
        {"A0", "Как дела?", "How are you?"},
        {"A1", "Где находится библиотека?", "Where is the library?"},
        {"A1", "Я люблю читать книги.", "I like to read books."},
        {"A2", "Что ты делал вчера вечером?", "What did you do last night?"},
        {"A2", "Я планирую поехать в отпуск.", "I am planning to go on vacation."},
        {"B1", "Несмотря на трудности, мы достигли успеха.", "Despite the difficulties, we achieved success."},
        {"B1", "Я уверен, что он придет вовремя.", "I am sure that he will come on time."},
        {"B2", "Общество должно стремиться к устойчивому развитию.", "Society should strive for sustainable development."},
        {"B2", "Он известен своим новаторским подходом к решению проблем.", "He is known for his innovative approach to problem-solving."},
        {"C1", "Ввиду вышеизложенного, необходимо принять незамедлительные меры.", "In view of the foregoing, immediate action is necessary."},
        {"C1", "Его аргументы были весьма убедительны, хотя и несколько спорны.", "His arguments were quite compelling, albeit somewhat controversial."},
        {"A0", "Меня зовут ...", "My name is ..."},
        {"A0", "Сколько тебе лет?", "How old are you?"},
        {"A0", "Что это?", "What is this?"},
        {"A0", "До свидания!", "Goodbye!"},
        {"A0", "Спасибо!", "Thank you!"},
        {"A0", "Пожалуйста.", "You're welcome."},
        {"A0", "Да.", "Yes."},
        {"A0", "Нет.", "No."},
        {"A0", "Здравствуйте!", "Hello!"},
        {"A0", "Как дела?", "How are you?"},
        {"A0", "Меня зовут...", "My name is..."},
        {"A0", "Сколько тебе лет?", "How old are you?"},
        {"A0", "Что это?", "What is this?"},
        {"A0", "До свидания!", "Goodbye!"},
        {"A0", "Спасибо!", "Thank you!"},
        {"A0", "Пожалуйста.", "You're welcome."},
        {"A0", "Да.", "Yes."},
        {"A0", "Нет.", "No."},
        {"A0", "Я хорошо.", "I am fine."},
        {"A0", "Как тебя зовут?", "What is your name?"},
        {"A0", "Мне ... лет.", "I am ... years old."},
        {"A0", "Это книга.", "This is a book."},
        {"A0", "Я из России.", "I am from Russia."},

        {"A1", "Где ты живешь?", "Where do you live?"},
        {"A1", "Я люблю читать книги.", "I like to read books."},
        {"A1", "Это моя семья.", "This is my family."},
        {"A1", "У меня есть собака.", "I have a dog."},
        {"A1", "Я учу английский язык.", "I am learning English."},
        {"A1", "Он играет в футбол.", "He plays football."},
        {"A1", "Она любит музыку.", "She likes music."},
        {"A1", "Мы ходим в школу.", "We go to school."},
        {"A1", "Они живут в Москве.", "They live in Moscow."},
        {"A1", "Я голоден.", "I am hungry."},
        {"A1", "Где ты живешь?", "Where do you live?"},
        {"A1", "Я люблю читать книги.", "I like to read books."},
        {"A1", "Это моя семья.", "This is my family."},
        {"A1", "У меня есть собака.", "I have a dog."},
        {"A1", "Я учу английский язык.", "I am learning English."},
        {"A1", "Он играет в футбол.", "He plays football."},
        {"A1", "Она любит музыку.", "She likes music."},
        {"A1", "Мы ходим в школу.", "We go to school."},
        {"A1", "Они живут в Москве.", "They live in Moscow."},
        {"A1", "Я голоден.", "I am hungry."},
        {"A1", "Она говорит по-русски.", "She speaks Russian."},
        {"A1", "Мы идем в магазин.", "We are going to the store."},
        {"A1", "Он работает врачом.", "He works as a doctor."},
        {"A1", "Я люблю путешествовать.", "I like to travel."},
        {"A1", "Они смотрят телевизор.", "They are watching TV."},

        {"A2", "Что ты делал вчера?", "What did you do yesterday?"},
        {"A2", "Я хочу пойти в кино.", "I want to go to the cinema."},
        {"A2", "Она говорит по-английски.", "She speaks English."},
        {"A2", "Мы посетили музей.", "We visited the museum."},
        {"A2", "Они планируют путешествие.", "They are planning a trip."},
        {"A2", "Мне нужно купить продукты.", "I need to buy groceries."},
        {"A2", "Он работает в офисе.", "He works in an office."},
        {"A2", "Она читает газету.", "She is reading a newspaper."},
        {"A2", "Мы смотрели телевизор.", "We were watching television."},
        {"A2", "Они слушали радио.", "They were listening to the radio."},
        {"A2", "Что ты делал вчера?", "What did you do yesterday?"},
        {"A2", "Я хочу пойти в кино.", "I want to go to the cinema."},
        {"A2", "Она говорит по-английски.", "She speaks English."},
        {"A2", "Мы посетили музей.", "We visited the museum."},
        {"A2", "Они планируют путешествие.", "They are planning a trip."},
        {"A2", "Мне нужно купить продукты.", "I need to buy groceries."},
        {"A2", "Он работает в офисе.", "He works in an office."},
        {"A2", "Она читает газету.", "She is reading a newspaper."},
        {"A2", "Мы смотрели телевизор.", "We were watching television."},
        {"A2", "Они слушали радио.", "They were listening to the radio."},
        {"A2", "Я ходил в спортзал.", "I went to the gym."},
        {"A2", "Она приготовила обед.", "She cooked lunch."},
        {"A2", "Мы играли в игры.", "We played games."},
        {"A2", "Он читал книгу.", "He was reading a book."},
        {"A2", "Они изучали математику.", "They were studying mathematics."},

        {"B1", "Я думаю, это хорошая идея.", "I think it's a good idea."},
        {"B1", "Он всегда опаздывает.", "He is always late."},
        {"B1", "Мы должны это сделать.", "We should do it."},
        {"B1", "Я не уверен.", "I am not sure."},
        {"B1", "Это зависит от обстоятельств.", "It depends on the circumstances."},
        {"B1", "Он очень талантливый.", "He is very talented."},
        {"B1", "Она очень умная.", "She is very smart."},
        {"B1", "Мы хорошо провели время.", "We had a good time."},
        {"B1", "Они живут далеко отсюда.", "They live far from here."},
        {"B1", "Я люблю гулять в парке.", "I like to walk in the park."},
        {"B1", "Я думаю, это хорошая идея.", "I think it's a good idea."},
        {"B1", "Он всегда опаздывает.", "He is always late."},
        {"B1", "Мы должны это сделать.", "We should do it."},
        {"B1", "Я не уверен.", "I am not sure."},
        {"B1", "Это зависит от обстоятельств.", "It depends on the circumstances."},
        {"B1", "Он очень талантливый.", "He is very talented."},
        {"B1", "Она очень умная.", "She is very smart."},
        {"B1", "Мы хорошо провели время.", "We had a good time."},
        {"B1", "Они живут далеко отсюда.", "They live far from here."},
        {"B1", "Я люблю гулять в парке.", "I like to walk in the park."},
        {"B1", "Я думаю о тебе.", "I am thinking of you."},
        {"B1", "Она заботится о нем.", "She cares about him."},
        {"B1", "Мы верим в тебя.", "We believe in you."},
        {"B1", "Он ждет тебя.", "He is waiting for you."},
        {"B1", "Они гордятся тобой.", "They are proud of you."},

        {"B2", "Несмотря на дождь, мы пошли гулять.", "Despite the rain, we went for a walk."},
        {"B2", "У меня нет времени.", "I don't have time."},
        {"B2", "Она сказала, что устала.", "She said that she was tired."},
        {"B2", "Я был удивлен, когда услышал эту новость.", "I was surprised when I heard the news."},
        {"B2", "Он предложил помочь мне.", "He offered to help me."},
        {"B2", "Она отказалась отвечать на вопросы.", "She refused to answer the questions."},
        {"B2", "Мы надеемся, что все будет хорошо.", "We hope everything will be alright."},
        {"B2", "Они уверены в своем успехе.", "They are confident in their success."},
        {"B2", "Я благодарен вам за вашу помощь.", "I am grateful to you for your help."},
        {"B2", "Он сожалеет о своем поступке.", "He regrets his action."},
        {"B2", "Несмотря на дождь, мы пошли гулять.", "Despite the rain, we went for a walk."},
        {"B2", "У меня нет времени.", "I don't have time."},
        {"B2", "Она сказала, что устала.", "She said that she was tired."},
        {"B2", "Я был удивлен, когда услышал эту новость.", "I was surprised when I heard the news."},
        {"B2", "Он предложил помочь мне.", "He offered to help me."},
        {"B2", "Она отказалась отвечать на вопросы.", "She refused to answer the questions."},
        {"B2", "Мы надеемся, что все будет хорошо.", "We hope everything will be alright."},
        {"B2", "Они уверены в своем успехе.", "They are confident in their success."},
        {"B2", "Я благодарен вам за вашу помощь.", "I am grateful to you for your help."},
        {"B2", "Он сожалеет о своем поступке.", "He regrets his action."},
        {"B2", "Я с нетерпением жду нашей встречи.", "I am looking forward to our meeting."},
        {"B2", "Она привыкла к трудностям.", "She is used to difficulties."},
        {"B2", "Мы уверены в нашей победе.", "We are certain of our victory."},
        {"B2", "Он отвечает за этот проект.", "He is responsible for this project."},
        {"B2", "Она обладает хорошим чувством юмора.", "She has a good sense of humor."},

        {"C1", "В связи с вышеизложенным, мы приняли решение.", "In light of the foregoing, we have made a decision."},
        {"C1", "С учетом всех обстоятельств...", "Taking everything into account..."},
        {"C1", "Это вопрос первостепенной важности.", "This is a matter of paramount importance."},
        {"C1", "Нельзя отрицать, что...", "It cannot be denied that..."},
        {"C1", "Он был вынужден признать свою ошибку.", "He was compelled to admit his mistake."},
        {"C1", "Она проявила необычайную храбрость.", "She demonstrated extraordinary courage."},
        {"C1", "Мы должны принять во внимание все факторы.", "We must take all factors into consideration."},
        {"C1", "Они стремятся к достижению совершенства.", "They strive for excellence."},
        {"C1", "Я не могу выразить словами свою благодарность.", "I cannot express my gratitude in words."},
        {"C1", "Он обладает выдающимся интеллектом.", "He possesses outstanding intelligence."},
        {"C1", "В связи с вышеизложенным, мы приняли решение.", "In light of the foregoing, we have made a decision."},
        {"C1", "С учетом всех обстоятельств...", "Taking everything into account..."},
        {"C1", "Это вопрос первостепенной важности.", "This is a matter of paramount importance."},
        {"C1", "Нельзя отрицать, что...", "It cannot be denied that..."},
        {"C1", "Он был вынужден признать свою ошибку.", "He was compelled to admit his mistake."},
        {"C1", "Она проявила необычайную храбрость.", "She demonstrated extraordinary courage."},
        {"C1", "Мы должны принять во внимание все факторы.", "We must take all factors into consideration."},
        {"C1", "Они стремятся к достижению совершенства.", "They strive for excellence."},
        {"C1", "Я не могу выразить словами свою благодарность.", "I cannot express my gratitude in words."},
        {"C1", "Он обладает выдающимся интеллектом.", "He possesses outstanding intelligence."},
        {"C1", "Эта проблема требует незамедлительного решения.", "This problem requires an immediate solution."},
        {"C1", "Ее талант неоспорим.", "Her talent is undeniable."},
        {"C1", "Мы полны решимости добиться успеха.", "We are determined to succeed."},
        {"C1", "Он столкнулся с неразрешимой дилеммой.", "He was faced with an insurmountable dilemma."},
        {"C1", "Она внесла неоценимый вклад в этот проект.", "She made an invaluable contribution to this project."}
    };

    QString insertSql = R"(
        INSERT INTO translation_questions (level, question_ru, answer_en)
        VALUES (:level, :question_ru, :answer_en)
    )";

    query.prepare(insertSql);

    for (const auto& question : questions) {
        query.bindValue(":level", std::get<0>(question));
        query.bindValue(":question_ru", std::get<1>(question));
        query.bindValue(":answer_en", std::get<2>(question));

        if (!query.exec()) {
            qDebug() << "Ошибка при вставке строки в translation_questions:" << query.lastError().text();
            db.close();
            QCoreApplication::exit(1);
            return;
        }
    }

    qDebug() << "Таблица translation_questions успешно заполнена.";
    db.close();
}

void populateExpressionQuestions(const QString& dbName = "english_test3.db") {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);

    if (!db.open()) {
        qDebug() << "Не удалось открыть базу данных:" << db.lastError().text();
        QCoreApplication::exit(1);
        return;
    }

    QSqlQuery query(db);

    std::vector<std::tuple<QString, QString, QString>> questions = {
        {"A0", "What is your name?", "My name is ..."},
        {"A1", "Where are you from?", "I am from ..."},
        {"A2", "Describe your family.", "I have ... family members.  ... is/are ..."},
        {"B1", "What are the advantages and disadvantages of living in a big city?", "Advantages: ..., Disadvantages: ..."},
        {"B2", "Discuss the impact of social media on society.", "Social media has a significant impact on ...  It ..."},
        {"C1", "To what extent do you agree with the statement: 'The best way to learn a language is to live in a country where it is spoken'?", "I agree to a great extent/ I partially agree/ I disagree."},
        {"A0", "What is your name?", "My name is ..."},
        {"A0", "How old are you?", "I am ... years old."},
        {"A0", "What is your favorite color?", "My favorite color is ..."},
        {"A0", "What do you like to eat?", "I like to eat ..."},
        {"A0", "Where are you from?", "I am from ..."},
        {"A0", "How are you?", "I am fine, thank you."},
        {"A0", "What is this?", "This is a ..."},
        {"A0", "What is your job?", "I am a ..."},
        {"A0", "Do you have any pets?", "Yes, I have ... / No, I don't."},
        {"A0", "What is your favorite animal?", "My favorite animal is ..."},
        {"A0", "What is your name?", "My name is ..."},
        {"A0", "How old are you?", "I am ... years old."},
        {"A0", "What is your favorite color?", "My favorite color is ..."},
        {"A0", "What do you like to eat?", "I like to eat ..."},
        {"A0", "Where are you from?", "I am from ..."},
        {"A0", "How are you?", "I am fine, thank you."},
        {"A0", "What is this?", "This is a ..."},
        {"A0", "What is your job?", "I am a ..."},
        {"A0", "Do you have any pets?", "Yes, I have ... / No, I don't."},
        {"A0", "What is your favorite animal?", "My favorite animal is ..."},
        {"A0", "What is your favorite toy?", "My favorite toy is a ..."},
        {"A0", "What do you like to wear?", "I like to wear ..."},
        {"A0", "What is your favorite game?", "My favorite game is ..."},
        {"A0", "What is your favorite day of the week?", "My favorite day is ..."},
        {"A0", "What do you want to be when you grow up?", "I want to be a ..."},

        {"A1", "Where are you from?", "I am from ..."},
        {"A1", "What do you like to do in your free time?", "I like to ..."},
        {"A1", "What is your favorite food?", "My favorite food is ..."},
        {"A1", "Describe your family.", "I have ... family members. They are ..."},
        {"A1", "What do you do every day?", "I ... every day."},
        {"A1", "What is your favorite book?", "My favorite book is ..."},
        {"A1", "What is your favorite movie?", "My favorite movie is ..."},
        {"A1", "Describe your best friend.", "My best friend is ... . He/She is ..."},
        {"A1", "What are your hobbies?", "My hobbies are ..."},
        {"A1", "What is the weather like today?", "The weather is ..."},
        {"A1", "Where are you from?", "I am from ..."},
        {"A1", "What do you like to do in your free time?", "I like to ..."},
        {"A1", "What is your favorite food?", "My favorite food is ..."},
        {"A1", "Describe your family.", "I have ... family members. They are ..."},
        {"A1", "What do you do every day?", "I ... every day."},
        {"A1", "What is your favorite book?", "My favorite book is ..."},
        {"A1", "What is your favorite movie?", "My favorite movie is ..."},
        {"A1", "Describe your best friend.", "My best friend is ... . He/She is ..."},
        {"A1", "What are your hobbies?", "My hobbies are ..."},
        {"A1", "What is the weather like today?", "The weather is ..."},
        {"A1", "What did you do on your last birthday?", "On my last birthday, I ..."},
        {"A1", "Describe your school.", "My school is ..."},
        {"A1", "What do you want to do this summer?", "This summer, I want to ..."},
        {"A1", "What are you wearing today?", "Today I am wearing ..."},
        {"A1", "What is your favorite sport?", "My favorite sport is ..."},

        {"A2", "Describe your typical day.", "I wake up at ..., then ..."},
        {"A2", "What are your plans for the weekend?", "I plan to ..."},
        {"A2", "What did you do last weekend?", "Last weekend, I ..."},
        {"A2", "Describe a place you like to visit.", "I like to visit ... . It is ... because ..."},
        {"A2", "What are your favorite subjects at school?", "My favorite subjects are ..."},
        {"A2", "Talk about your best friend.", "My best friend is ... . We ... together."},
        {"A2", "What are you going to do tonight?", "Tonight, I am going to ..."},
        {"A2", "Describe your ideal vacation.", "My ideal vacation would be ..."},
        {"A2", "What are you good at?", "I am good at ..."},
        {"A2", "Talk about a famous person you admire.", "I admire ... because ..."},
        {"A2", "Describe your typical day.", "I wake up at ..., then ..."},
        {"A2", "What are your plans for the weekend?", "I plan to ..."},
        {"A2", "What did you do last weekend?", "Last weekend, I ..."},
        {"A2", "Describe a place you like to visit.", "I like to visit ... . It is ... because ..."},
        {"A2", "What are your favorite subjects at school?", "My favorite subjects are ..."},
        {"A2", "Talk about your best friend.", "My best friend is ... . We ... together."},
        {"A2", "What are you going to do tonight?", "Tonight, I am going to ..."},
        {"A2", "Describe your ideal vacation.", "My ideal vacation would be ..."},
        {"A2", "What are you good at?", "I am good at ..."},
        {"A2", "Talk about a famous person you admire.", "I admire ... because ..."},
        {"A2", "Describe a book you recently read.", "I recently read ... It was about ..."},
        {"A2", "What are the advantages and disadvantages of living in the countryside?", "The advantages are ..., the disadvantages are ..."},
        {"A2", "Describe your favorite season.", "My favorite season is ... because ..."},
        {"A2", "What are your favorite things to do with your family?", "I like to ... with my family."},
        {"A2", "What is your favorite holiday?", "My favorite holiday is ..."},

        {"B1", "What are the advantages and disadvantages of living in a big city?", "Advantages: ..., Disadvantages: ..."},
        {"B1", "Describe your perfect vacation.", "My perfect vacation would be ..."},
        {"B1", "What are your plans for the future?", "I plan to ..."},
        {"B1", "What are the benefits of learning a foreign language?", "Benefits include ..."},
        {"B1", "Talk about a problem in your local area and suggest a solution.", "The problem is ...  I suggest ..."},
        {"B1", "Describe a memorable experience.", "A memorable experience was when ..."},
        {"B1", "What is your opinion about social media?", "In my opinion, ..."},
        {"B1", "What are the benefits and drawbacks of working from home?", "Benefits: ..., Drawbacks: ..."},
        {"B1", "What are the advantages and disadvantages of online shopping?", "Advantages: ..., Disadvantages: ..."},
        {"B1", "Describe a person who has influenced you.", "A person who has influenced me is ..."},
        {"B1", "What are the advantages and disadvantages of living in a big city?", "Advantages: ..., Disadvantages: ..."},
        {"B1", "Describe your perfect vacation.", "My perfect vacation would be ..."},
        {"B1", "What are your plans for the future?", "I plan to ..."},
        {"B1", "What are the benefits of learning a foreign language?", "Benefits include ..."},
        {"B1", "Talk about a problem in your local area and suggest a solution.", "The problem is ...  I suggest ..."},
        {"B1", "Describe a memorable experience.", "A memorable experience was when ..."},
        {"B1", "What is your opinion about social media?", "In my opinion, ..."},
        {"B1", "What are the benefits and drawbacks of working from home?", "Benefits: ..., Drawbacks: ..."},
        {"B1", "What are the advantages and disadvantages of online shopping?", "Advantages: ..., Disadvantages: ..."},
        {"B1", "Describe a person who has influenced you.", "A person who has influenced me is ..."},
        {"B1", "What are the environmental problems in your area?", "The environmental problems are ..."},
        {"B1", "Describe a time when you helped someone.", "I helped someone when ..."},
        {"B1", "What are the qualities of a good friend?", "The qualities of a good friend are ..."},
        {"B1", "What are your views on education?", "My views on education are ..."},
        {"B1", "What are the benefits and drawbacks of technology?", "Benefits: ..., Drawbacks: ..."},

        {"B2", "Discuss the impact of social media on society.", "Social media has a significant impact on ...  It ..."},
        {"B2", "What are the challenges facing the world today?", "Some of the challenges facing the world today are ..."},
        {"B2", "Describe a time when you overcame a difficult challenge.", "I overcame a difficult challenge when ..."},
        {"B2", "What are the benefits and drawbacks of globalization?", "Benefits: ..., Drawbacks: ..."},
        {"B2", "To what extent do you agree that technology has made our lives easier?", "I agree that ... because ..."},
        {"B2", "Discuss the importance of education.", "Education is important because ..."},
        {"B2", "What are the advantages and disadvantages of working in a team?", "Advantages: ..., Disadvantages: ..."},
        {"B2", "What are the benefits of travelling?", "Travelling offers several benefits, such as ..."},
        {"B2", "Discuss the role of the media in modern society.", "The media plays a crucial role in ..."},
        {"B2", "What are the causes and effects of climate change?", "Causes include ..., effects are ..."},
        {"B2", "Discuss the impact of social media on society.", "Social media has a significant impact on ...  It ..."},
        {"B2", "What are the challenges facing the world today?", "Some of the challenges facing the world today are ..."},
        {"B2", "Describe a time when you overcame a difficult challenge.", "I overcame a difficult challenge when ..."},
        {"B2", "What are the benefits and drawbacks of globalization?", "Benefits: ..., Drawbacks: ..."},
        {"B2", "To what extent do you agree that technology has made our lives easier?", "I agree that ... because ..."},
        {"B2", "Discuss the importance of education.", "Education is important because ..."},
        {"B2", "What are the advantages and disadvantages of working in a team?", "Advantages: ..., Disadvantages: ..."},
        {"B2", "What are the benefits of travelling?", "Travelling offers several benefits, such as ..."},
        {"B2", "Discuss the role of the media in modern society.", "The media plays a crucial role in ..."},
        {"B2", "What are the causes and effects of climate change?", "Causes include ..., effects are ..."},
        {"B2", "Discuss the importance of preserving cultural heritage.", "Preserving cultural heritage is important because ..."},
        {"B2", "To what extent do you think advertising influences people's choices?", "I think advertising influences people's choices to a certain extent because ..."},
        {"B2", "Discuss the importance of healthy lifestyle.", "A healthy lifestyle is important because ..."},
        {"B2", "What are your views on capital punishment?", "My views on capital punishment are ..."},
        {"B2", "Discuss the importance of renewable energy sources.", "Renewable energy sources are important because ..."},

        {"C1", "To what extent do you agree with the statement: 'The best way to learn a language is to live in a country where it is spoken'?", "I agree to a great extent/ I partially agree/ I disagree."},
        {"C1", "Assess the role of artificial intelligence in modern society.", "AI plays a significant role in ...  It ..."},
        {"C1", "Discuss the ethical considerations of genetic engineering.", "Genetic engineering raises ethical concerns about ..."},
        {"C1", "Evaluate the impact of social inequality on society.", "Social inequality has a profound impact on ..."},
        {"C1", "To what extent is censorship justifiable?", "Censorship is justifiable to a certain extent ..."},
        {"C1", "Discuss the benefits and drawbacks of a cashless society.", "Benefits: ..., Drawbacks: ..."},
        {"C1", "Assess the impact of urbanization on the environment.", "Urbanization has significant environmental consequences, such as ..."},
        {"C1", "To what extent is consumerism damaging to society?", "Consumerism is damaging to society because ..."},
        {"C1", "Discuss the importance of cultural diversity.", "Cultural diversity is important because ..."},
        {"C1", "Evaluate the effectiveness of current approaches to tackling poverty.", "Current approaches to tackling poverty are ..."},
        {"C1", "To what extent do you agree with the statement: 'The best way to learn a language is to live in a country where it is spoken'?", "I agree to a great extent/ I partially agree/ I disagree."},
        {"C1", "Assess the role of artificial intelligence in modern society.", "AI plays a significant role in ...  It ..."},
        {"C1", "Discuss the ethical considerations of genetic engineering.", "Genetic engineering raises ethical concerns about ..."},
        {"C1", "Evaluate the impact of social inequality on society.", "Social inequality has a profound impact on ..."},
        {"C1", "To what extent is censorship justifiable?", "Censorship is justifiable to a certain extent ..."},
        {"C1", "Discuss the benefits and drawbacks of a cashless society.", "Benefits: ..., Drawbacks: ..."},
        {"C1", "Assess the impact of urbanization on the environment.", "Urbanization has significant environmental consequences, such as ..."},
        {"C1", "To what extent is consumerism damaging to society?", "Consumerism is damaging to society because ..."},
        {"C1", "Discuss the importance of cultural diversity.", "Cultural diversity is important because ..."},
        {"C1", "Evaluate the effectiveness of current approaches to tackling poverty.", "Current approaches to tackling poverty are ..."},
        {"C1", "Discuss the ethical implications of autonomous vehicles.", "Autonomous vehicles raise ethical questions about ..."},
        {"C1", "To what extent should governments regulate the internet?", "Governments should regulate the internet to a certain extent ..."},
        {"C1", "Discuss the benefits and drawbacks of space exploration.", "Benefits: ..., Drawbacks: ..."},
        {"C1", "Evaluate the role of international organizations in maintaining global peace.", "International organizations play a vital role in ..."},
        {"C1", "To what extent is it possible to balance economic growth with environmental protection?", "Balancing economic growth with environmental protection is challenging but possible if ..."}
    };

    QString insertSql = R"(
        INSERT INTO expression_questions (level, prompt, correct_answer)
        VALUES (:level, :prompt, :correct_answer)
    )";

    query.prepare(insertSql);

    for (const auto& question : questions) {
        query.bindValue(":level", std::get<0>(question));
        query.bindValue(":prompt", std::get<1>(question));
        query.bindValue(":correct_answer", std::get<2>(question));

        if (!query.exec()) {
            qDebug() << "Ошибка при вставке строки в expression_questions:" << query.lastError().text();
            db.close();
            QCoreApplication::exit(1);
            return;
        }
    }

    qDebug() << "Таблица expression_questions успешно заполнена.";
    db.close();
}

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    createDatabase();
    populateGrammarQuestions();
    populateTranslationQuestions();
    populateExpressionQuestions();

    return a.exec();
}
