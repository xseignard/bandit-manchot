<template>
  <div>
    <b-navbar type="is-primary" transparent :mobile-burger="false">
      <template slot="brand">
        <b-navbar-item>
          One arm crypto bandit by
          <a
            class="distrib"
            href="https://www.distributedgallery.com/"
            target="_blank"
          >
            distributed gallery/
          </a>
        </b-navbar-item>
      </template>
    </b-navbar>
    <div class="container">
      <form @submit.prevent="updateSettings">
        <b-field label="SSID">
          <b-input v-model="ssid" required icon="wifi"></b-input>
        </b-field>
        <b-field label="Password">
          <b-input
            v-model="password"
            icon="key"
            type="password"
            password-reveal
          ></b-input>
        </b-field>
        <div class="buttons">
          <b-button type="is-primary" native-type="submit" :disabled="!valid">
            Update
          </b-button>
          <b-button type="is-danger" @click.prevent="reset" :disabled="!valid">
            Reset
          </b-button>
        </div>
      </form>
    </div>
    <b-loading class="loader-layer" :active="!loaded"></b-loading>
    <b-loading class="toast-layer" :active="toast"></b-loading>
  </div>
</template>

<script>
export default {
  name: 'App',
  data: () => ({
    valid: true,
    toast: false,
    loaded: false,
    serverUrl:
      process.env.NODE_ENV === 'production'
        ? window.location.href
        : 'http://localhost:3000/',
    // v-model default values
    // will be overwritten by the settings api call
    ssid: '',
    password: '',
  }),
  mounted() {
    this.init()
  },
  watch: {
    startUniverse() {
      this.validate()
    },
    nodeName() {
      this.validate()
    },
  },
  methods: {
    validate() {
      if (this.ssid) this.valid = true
      else this.valid = false
    },
    async init() {
      const res = await fetch(`${this.serverUrl}settings`)
      const { ssid, password } = await res.json()
      this.ssid = ssid
      this.password = password
      setTimeout(() => {
        this.loaded = true
      }, 1000)
    },
    async reset() {
      this.ssid = ''
      this.password = ''
      await this.updateSettings()
    },
    async updateSettings() {
      await fetch(`${this.serverUrl}update`, {
        method: 'POST',
        headers: {
          Accept: 'application/json',
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({
          ssid: this.ssid,
          password: this.password,
        }),
      })
      this.toast = true
      this.$buefy.toast.open({
        duration: 20000,
        message: `${this.nodeName} will restart! (this page will reload in 15s)`,
        position: 'is-bottom',
        type: 'is-danger',
      })
      setTimeout(() => {
        window.location.reload()
      }, 15000)
    },
  },
}
</script>

<style lang="css" scoped>
.distrib {
  color: #ffffff;
  text-decoration: underline;
  font-weight: bold;
  margin-left: 5px;
}
.container {
  margin-top: 30px;
  padding: 0 10px;
}
.buttons {
  display: flex;
  justify-content: flex-end;
}
.loader-layer,
.toast-layer {
  background-color: #ffffff;
  z-index: 5;
}
.toast-layer {
  opacity: 0.7;
}
</style>
