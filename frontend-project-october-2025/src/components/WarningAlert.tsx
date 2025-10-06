import Alert from '@mui/material/Alert';

export default function AlertSnackbar({ alertType }: { alertType: 'h' | 't' }) {
	const message =
		alertType === 't'
			? 'Warning! High temperature detected.'
			: 'Warning! High humidity detected.';

	return (
		<div className="alertBox">
			<Alert severity="warning" variant="filled" sx={{ width: '100%' }}>
				{message}
			</Alert>
		</div>
	);
}
