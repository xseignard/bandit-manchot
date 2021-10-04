const express = require('express')
const cors = require('cors')

const app = express()

app.use(cors())
app.use(express.json())

let settings = {
  ssid: 'test',
  password: 'password',
}

app.get('/settings', (req, res) => {
  res.json(settings)
})

app.post('/update', (req, res) => {
  settings = req.body
  res.json({ ok: true })
})

app.listen(3000, () => {
  console.log('dev server started')
})
