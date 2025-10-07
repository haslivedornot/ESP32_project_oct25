import { Box } from '@mui/material';
import { DataGrid, type GridColDef } from '@mui/x-data-grid';
import type { DataGridRow } from '../models/temperature';

export const TempTable = ({ dataset }: { dataset: DataGridRow[] }) => {
	const columns: GridColDef<DataGridRow>[] = [
		{ field: 'id', headerName: 'ID', width: 50 },
		{
			field: 'time',
			headerName: 'Time',
			width: 100,
			valueFormatter: (value: number) => {
				return new Date(value).toLocaleTimeString('en-US', {
					hour: '2-digit',
					minute: '2-digit',
				});
			},
		},
		{
			field: 'temperature',
			headerName: 'Temperature',
			width: 100,
		},
		{
			field: 'humidity',
			headerName: 'Humidity',
			width: 100,
		},
	];
	return (
		<Box sx={{ height: '100%', width: 'fit-content' }}>
			<DataGrid
				sx={{
					padding: '5px',
				}}
				rows={dataset}
				columns={columns}
				rowSelection={false}
				hideFooterPagination
				hideFooterSelectedRowCount
				hideFooter
				disableRowSelectionOnClick
			/>
		</Box>
	);
};
