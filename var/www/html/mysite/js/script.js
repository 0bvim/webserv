console.log("Quebrou");


const requestPostJson = {
    method: 'POST',
    headers: {
        'Content-Type': 'application/json'
    },
    body: JSON.stringify({id: 1})
}

fetch("http://127.0.0.1:8090/delete/json", requestPostJson);




let variavel = 1;