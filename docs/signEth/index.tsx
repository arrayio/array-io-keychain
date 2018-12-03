import * as React from 'react';
import { render } from 'react-dom';
import Root from './app/components/Root';

const rootElement = document.getElementById('root');

if (rootElement) {
  render(<Root />, rootElement);
}
