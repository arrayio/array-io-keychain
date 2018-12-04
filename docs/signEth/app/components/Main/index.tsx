import * as React from 'react';
 
import './styles.css';

interface MainState {
  result: string;
  inputValue: string;
}

export default class Main extends React.Component<{}, MainState> {
  constructor(props: any) {
    super(props);
    this.handleSignClick = this.handleSignClick.bind(this);
    this.handlePublicKeyClick = this.handlePublicKeyClick.bind(this);
    this.state = {
      result: '',
      inputValue: '',
    };
  }

  async handleSignClick(e: any) {
    const result = 'keychain.sign(this.state.inputValue)';
    this.setState({result});
  }

  async handlePublicKeyClick(e: any) {
    const result = 'keychain.publicKey()';
    this.setState({result});
  }

  updateInputValue(evt: React.ChangeEvent<HTMLInputElement>) {
    this.setState({
      inputValue: evt.target.value,
    });
  }

  render() {
    return (
      <div className="container">
        <ul className="flex-outer">
          <li>
            <label>Transaction</label>
            <input
              type="text"
              placeholder="871689d060721b5cec5a010080841e00000000000011130065cd1d0000000000000000"
              onChange={evt => this.updateInputValue(evt)}
            />
          </li>
          <li>
            <button onClick={this.handlePublicKeyClick}>Public key</button>
            <button onClick={this.handleSignClick}>Sign</button>
            <span>{ this.state.result }</span>
          </li>
        </ul>
      </div>
    );
  }

}

