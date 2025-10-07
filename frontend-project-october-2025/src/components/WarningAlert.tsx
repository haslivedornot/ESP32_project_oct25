import Alert from '@mui/material/Alert';

export default function WarningAlert({
	alertType,
}: {
	alertType: 'h' | 't' | null;
}) {
	const message =
		alertType === 't'
			? 'Warning! High temperature detected.'
			: 'Warning! High humidity detected.';

	return (
		<div className="alertBox">
			{alertType == null ? (
				<></>
			) : (
				<Alert severity="warning" variant="filled" sx={{ width: '100%' }}>
					{message}
				</Alert>
			)}
		</div>
	);
}
