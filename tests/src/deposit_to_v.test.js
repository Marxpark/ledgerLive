import "core-js/stable";
import "regenerator-runtime/runtime";
import { waitForAppScreen, zemu, genericTx, nano_models,SPECULOS_ADDRESS, txFromEtherscan} from './test.fixture';
import { ethers } from "ethers";
import { parseEther, parseUnits} from "ethers/lib/utils";

// EDIT THIS: Replace with your contract address
const contractAddr = "0x08f4b84852cdfacbb2082913a1caeb90afe5357a";

const pluginName = "test";
const testNetwork = "ethereum";
const abi_path = `../networks/${testNetwork}/${pluginName}/abis/` + contractAddr + '.json';
const abi = require(abi_path);


nano_models.forEach(function(model) {
  test('[Nano ' + model.letter + '] Deposit to v', zemu(model, async (sim, eth) => {

  const serializedTx = txFromEtherscan("0x02f90232010e8459682f00850306dc420083092c5c9408f4b84852cdfacbb2082913a1caeb90afe5357a80b901c48ab936b800000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000699a0b0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000a000000000000000000000000f6157d5ff062aa7bfb783ca997b3c7dd18d366c8000000000000000000000000f619f9c3f86a4a7242343711474afd4997bc4fa5000000000000000000000000652930670a7fe06328bcfabc6dad13b76d13feb60000000000000000000000004505747d78dc1516d17d12911b0ceda9b4436da7000000000000000000000000a561972edb915788cdd9a291877373587f4e7aa7000000000000000000000000d9174b81f540272e2441a6237c0d61da32b4a73d000000000000000000000000e12286c9b881ea429cfd9e6ca0fb666c819981b000000000000000000000000094df827cadc7101452a24161d6bb2f6da2c6f69800000000000000000000000096cd104e480bebbfa5d07529e5ac42216e2eeeba00000000000000000000000030826399cf8f6d53e350d53f8d026db4f35bb498c001a022bedca437f978144c354d31103ada8714e41dc025a0b966c3be712060ee75afa04c99d42ce182a892524eef2cd5fa9c9596438294d75a160f2d1a7dfa41e4a077");

  const tx = eth.signTransaction(
    "44'/60'/0'/0",
    serializedTx,
  );

  const right_clicks = model.letter === 'S' ? 12 : 6;

  // Wait for the application to actually load and parse the transaction
  await waitForAppScreen(sim);
  // Navigate the display by pressing the right button `right_clicks` times, then pressing both buttons to accept the transaction.
  await sim.navigateAndCompareSnapshots('.', model.name + '_deposit_to_v', [right_clicks, 0]);

  await tx;
  }));
});

// Test from constructed transaction
// EDIT THIS: build your own test
// nano_models.forEach(function(model) {
//   test('[Nano ' + model.letter + '] Swap Exact Eth For Tokens', zemu(model, async (sim, eth) => {
//   const contract = new ethers.Contract(contractAddr, abi);
//
//   // Constants used to create the transaction
//   // EDIT THIS: Remove what you don't need
//   const amountOutMin = parseUnits("28471151959593036279", 'wei');
//   const WETH = "0xc02aaa39b223fe8d0a0e5c4f27ead9083c756cc2";
//   const SUSHI = "0x6b3595068778dd592e39a122f4f5a5cf09c90fe2";
//   const path = [WETH, SUSHI];
//   const deadline = Number(1632843280);
//   // We set beneficiary to the default address of the emulator, so it maches sender address
//   const beneficiary = SPECULOS_ADDRESS;
//
//   // EDIT THIS: adapt the signature to your method
//   // signature: swapExactETHForTokens(uint amountOutMin, address[] calldata path, address to, uint deadline)
//   // EDIT THIS: don't call `swapExactETHForTokens` but your own method and adapt the arguments.
//   const {data} = await contract.populateTransaction.swapExactETHForTokens(amountOutMin, path, beneficiary ,deadline);
//
//   // Get the generic transaction template
//   let unsignedTx = genericTx;
//   // Modify `to` to make it interact with the contract
//   unsignedTx.to = contractAddr;
//   // Modify the attached data
//   unsignedTx.data = data;
//   // EDIT THIS: get rid of this if you don't wish to modify the `value` field.
//   // Modify the number of ETH sent
//   unsignedTx.value = parseEther("0.1");
//
//   // Create serializedTx and remove the "0x" prefix
//   const serializedTx = ethers.utils.serializeTransaction(unsignedTx).slice(2);
//
//   const tx = eth.signTransaction(
//     "44'/60'/0'/0",
//     serializedTx
//   );
//
//   const right_clicks = model.letter === 'S' ? 7 : 5;
//
//   // Wait for the application to actually load and parse the transaction
//   await waitForAppScreen(sim);
//   // Navigate the display by pressing the right button 10 times, then pressing both buttons to accept the transaction.
//   // EDIT THIS: modify `10` to fix the number of screens you are expecting to navigate through.
//   await sim.navigateAndCompareSnapshots('.', model.name + '_swap_exact_eth_for_tokens', [right_clicks, 0]);
//
//   await tx;
//   }));
// });

