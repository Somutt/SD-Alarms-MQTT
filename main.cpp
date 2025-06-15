#include <mqtt/async_client.h>
using namespace std;

// tópico do alarme para receber as informações e publicar no console
const string TOPIC = "sensors/alarm";
// indica a qualidade de serviço, a mensagem será enviada pelo menos 1 vez nesse caso
constexpr int QOS = 1;
// contador p/ para o laço no teste
int counter = 0;

// a classe Callback implementa mqtt:callback, ela fiscaliza eventos na conexão MQTT e reage
// a esse eventos com uma função de callback
class Callback final : public virtual mqtt::callback {
public:
    // o evento é uma mensagem recebida, ele irá imprimir no console o alarme
    void message_arrived(const mqtt::const_message_ptr msg) override {
        cout << "Mensagem recebida: " << endl
             << " tópico: " << msg->get_topic() << endl
             << " payload: " << msg->to_string() << endl
             << endl;
        counter++;
    }
};

int main() {
    // inicializa a função de callback
    Callback cb;
    mqtt::async_client client("tcp://localhost:1883", "client_alarms");
    client.set_callback(cb);
    //inicializa as opções padrões do MQTT paho
    mqtt::connect_options connOpts;
    connOpts.set_clean_session(true);

    try {
        // conecta no cliente MQTT e increve no tópico
        client.connect(connOpts)->wait();
        cout << "Conectado ao servidor" << endl;

        client.subscribe(TOPIC, QOS)->wait();
        cout << "Inscrito em: " << TOPIC << endl;

        while (counter < 4) {}

        client.disconnect()->wait();
        cout << "Desconectado do servidor" << endl;
    } catch (const mqtt::exception &exc) {
        cerr << "Erro: " << exc.what() << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}