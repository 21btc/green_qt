#include "wally.h"

#include <QRandomGenerator>
#include <QSet>
#include <wally_bip39.h>

namespace {

QStringList GetWordlist()
{
    QStringList wordlist;
    words* ws;
    bip39_get_wordlist(nullptr, &ws);
    wordlist.reserve(BIP39_WORDLIST_LEN);
    for (size_t i = 0; i < BIP39_WORDLIST_LEN; ++i) {
        char* w;
        bip39_get_word(ws, i, &w);
        wordlist.append(QString::fromLatin1(w));
        wally_free_string(w);
    }
    return wordlist;
}

QStringList g_wordlist{GetWordlist()};
QSet<QString> g_wordset{g_wordlist.begin(), g_wordlist.end()};

} // namespace

MnemonicEditorController::MnemonicEditorController(QObject *parent) : QObject(parent) {
    for (int i = 0; i < 27; i++) {
        m_words.append(new Word(this, i));
    }
    m_words.at(0)->setEnabled(true);
}

void MnemonicEditorController::setAutoComplete(bool auto_complete)
{
    if (m_auto_complete == auto_complete) return;
    m_auto_complete = auto_complete;
    emit autoCompleteChanged(m_auto_complete);
}

QQmlListProperty<Word> MnemonicEditorController::words() {
    return { this, &m_words };
}

QString MnemonicEditorController::update(int index, const QString& text)
{
    QString out = updateWord(index, text);
    update();
    return out;
}

QString MnemonicEditorController::updateWord(int index, const QString& text)
{
    if (index >= m_mnemonic_size) return text;

    QString diff;
    Word* word = m_words.at(index);
    if (text.startsWith(word->text())) {
        diff = text.mid(word->text().length());
    } else {
        diff = text;
    }

    if (!diff.isEmpty()) {
        auto words = diff.trimmed().split(QRegExp("\\s+"));
        if (words.length() == 12 || words.length() == 24 || words.length() == 27) {
            bool changed = false;
            for (int i = 0; i < words.length(); ++i) {
                changed = m_words.at(i)->setText(words.at(i)) || changed;
            }
            if (changed) {
                emit mnemonicChanged();
            }
            return m_words.at(index)->text();
        }
    }

    if (m_words.at(index)->setText(text)) {
        emit mnemonicChanged();
    }
    return m_words.at(index)->text();
}

void MnemonicEditorController::clear()
{
    for (int i = 0; i < 27; ++i) {
        m_words.at(i)->setText("");
    }
    update();
}

QStringList MnemonicEditorController::mnemonic() const
{
    QStringList result;
    for (int i = 0; i < m_mnemonic_size; ++i) {
        result.append(m_words.at(i)->text());
    }
    return result;
}

float MnemonicEditorController::progress() const
{
    int count = 0;
    for (int i = 0; i < m_mnemonic_size; ++i) {
        if (m_words.at(i)->valid()) count ++;
    }
    return static_cast<float>(count) / static_cast<float>(m_mnemonic_size);
}

void MnemonicEditorController::update()
{
    m_valid = true;
    bool enabled = true;
    int last_valid = 26;
    while (last_valid >= 0) {
        if (m_words.at(last_valid)->valid()) break;
        last_valid --;
    }
    for (int i = 0; i < m_mnemonic_size; ++i) {
        auto word = m_words.at(i);
        if (!m_words.at(i)->valid()) m_valid = false;
        word->setEnabled(enabled);
        enabled = enabled && word->valid();
    }
    emit mnemonicChanged();
}

void MnemonicEditorController::setMnemonicSize(int size)
{
    if (size==m_mnemonic_size) return;
    m_mnemonic_size = size;
    emit mnemonicSizeChanged(size);
}

Word::Word(MnemonicEditorController* controller, int index)
    : QObject(controller)
    , m_controller(controller)
    , m_index(index)
{}

bool Word::setText(QString text) {
    if (m_text == text) return false;

    // A suggestion is a word with same start as input text.
    QStringList suggestions;
    if (text.length() > 1) {
        for (QString word : g_wordlist) {
            if (word.startsWith(text)) {
                suggestions.append(word);
            }
        }
    }
    // Handle auto complete only if match is unique
    // and if new text increments current text so that
    // backspace works.
    if (m_controller->autoComplete()) {
        if (suggestions.length() == 1 && !m_text.startsWith(text)) {
            text = suggestions.at(0);
            suggestions.clear();
        }
    }
    if (m_suggestions != suggestions) {
        m_suggestions = suggestions;
        emit suggestionsChanged();
    }
    bool valid = g_wordset.contains(text);
    if (m_valid != valid) {
        m_valid = valid;
        emit validChanged(m_valid);
    }

    m_text = text;
    emit textChanged(m_text);

    return true;
}

void Word::setEnabled(bool enabled)
{
    if (m_enabled == enabled) return;
    m_enabled = enabled;
    emit enabledChanged(m_enabled);
}

QString Word::update(const QString& text)
{
    return m_controller->update(m_index, text);
}

template <typename T>
QList<T> range(T lower, T upper)
{
    QList<T> res;
    for (T i = lower; i < upper; ++i) {
        res.append(i);
    }
    return res;
}

template <typename T>
QList<T> shuffle(QList<T> list)
{
    QList<T> res;
    while (!list.isEmpty()) {
        res.append(list.takeAt(QRandomGenerator::global()->bounded(list.size())));
    }
    return res;
}

MnemonicQuizWord::MnemonicQuizWord(int index, QObject *parent)
    : QObject(parent)
    , m_index(index)
{
}

void MnemonicQuizWord::setValue(const QString &value)
{
    if (m_value == value) return;
    m_value = value;
    emit valueChanged(m_value);
}

void MnemonicQuizWord::setOptions(const QStringList &options)
{
    m_options = options;
    emit optionsChanged(m_options);
}

void MnemonicQuizWord::setEnabled(bool enabled)
{
    if (m_enabled == enabled) return;
    m_enabled = enabled;
    emit enabledChanged(m_enabled);
}

void MnemonicQuizWord::setCorrect(bool correct)
{
    if (m_correct == correct) return;
    m_correct = correct;
    emit correctChanged(m_correct);
}
