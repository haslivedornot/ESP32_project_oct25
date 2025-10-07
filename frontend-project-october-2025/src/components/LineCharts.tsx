import { LineChart } from '@mui/x-charts';
import type { DataGridRow } from '../models/temperature';

export const LineCharts = ({ dataset }: { dataset: DataGridRow[] }) => {
	return (
		<>
			<LineChart
				dataset={dataset}
				xAxis={[
					{
						dataKey: 'time',
						label: 'Time',
						scaleType: 'time',
						valueFormatter: (value: number) => {
							return new Date(value).toLocaleTimeString('en-US', {
								hour: '2-digit',
								minute: '2-digit',
							});
						},
					},
				]}
				yAxis={[{ min: 15, max: 60, label: 'Temperature (°C)' }]}
				series={[
					{
						dataKey: 'temperature',
						color: 'red',
						label: 'Temperature (°C)',
						showMark: false,
					},
				]}
				height={200}
			/>
			<LineChart
				dataset={dataset}
				xAxis={[
					{
						dataKey: 'time',
						label: 'Time',
						scaleType: 'time',
						valueFormatter: (value: number) => {
							return new Date(value).toLocaleTimeString('en-US', {
								hour: '2-digit',
								minute: '2-digit',
							});
						},
					},
				]}
				yAxis={[{ min: 0, max: 100, label: 'Humidity (%)' }]}
				series={[
					{
						dataKey: 'humidity',
						color: 'blue',
						label: 'Humidity (%)',
						showMark: false,
					},
				]}
				height={200}
			/>
		</>
	);
};
