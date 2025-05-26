// test delete request

const requestDeleteJson = {
	method: 'DELETE',
	headers: {
		'Content-Type': 'application/json'
	},
	body: JSON.stringify({id: 1})
}

const requestDeleteImage = {
	method: 'DELETE',
	headers: {}
}

fetch("http://127.0.0.1:8090/delete/json", requestDeleteJson);// request delet to Json
fetch("http://127.0.0.1:8090/delete/image", requestDeleteImage);// request delet to Json