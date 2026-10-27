# DayZ NPC 개발·진단 현황 — 공개 읽기 스냅샷

schemaVersion: 1.0.0
snapshotId: snapshot-20261027T185443179320Z-acd7bf8c
generatedAt: 2026-10-27T18:54:43.179320Z
sourceUpdatedAt: 2026-10-27T17:36:00.756451Z
staleAfter: 2026-10-28T17:36:00.756451Z

생성 시각은 실험 수행 시각이 아닙니다. 현재 UTC가 staleAfter 이후면 오래된 자료(stale)로 해석하세요.

## 1. 작업 목표
안정성·실제 화면 품질·성능을 독립 판정합니다. 원인 특정 전 새 보간이나 갱신 방식을 추측으로 적용하지 않습니다.

## 2. 실제 수행
첫 재접근 후보의8/64 실제 시험, 수정 B8FB의 엔진 회귀, 단일 NPC CPU/영상 A/B/B/A 진단을 보존했습니다.

## 3. 측정 결과
|조건|최종 도착|HIGH_HOLD 내 미도달|낙하 복귀|미션 p50/p95/p99|새 분기|
|---|---:|---:|---:|---|---:|
|8명·runtime-20261028-021529·180초|8/8|0명|10회|20.0/33.0/34.0ms|0회|
|64명·runtime-20261028-021953·300초|64/64|11명|77회|25.0/59.0/82.0ms|0회|

위 두 행은 인원/실행 길이가 달라 성능 전후 대조가 아닙니다. 각1회 상세진단이며 GPU/Present 지연이 아닙니다.
CPU run 반복 주기 A1/B1/B2/A2: 3.24/2.08/2.08/3.22초. 단일 논리 NPC/8부품, AI·이동OFF, 같은 VSyncON,15~70초 구간. A/B/B/A는 평균 시간 반응만 반복 확인했고 부드러움은 수용하지 않았습니다. 개별 잔차와 원시 수치는 지표 JSON에 있습니다.

## 4. 원인 분석
첫 후보는 Drive가 Follow 전에 이유 문자열을 지워 새 분기가 작동하지 않았습니다. B8FB는 명시적 거부 전달을 엔진 대역으로 검증했지만 실제 이동은 미실행입니다.
애니메이션의 정확한 표시 불일치 원인은 미확정입니다. 보간 입력800개에서 상한 포화가 없었으며 평균 속도 보정으로 끊김이 해결됐다는 가정도 지지되지 않았습니다.

## 5. 검증 수준
진단 성공·74개 보존 회귀 통과와 전체 실험 성공/최종 수용을 구분합니다. 현재 기능·시각 품질·성능 모두 최종 수용 전입니다. 20ms p95는 미달성 목표입니다.

## 6. 변경 사항
공개 스냅샷은 읽기 전용 정적 파일입니다. 게임 코드·PBO·실험 기록을 변경하지 않았습니다. sourceRunId/buildId/조건은 아래 JSON에서 확인할 수 있습니다.

## 7. 원시 증거 연결
이 페이지는 전체 원시 로그가 아닌 선정된 공개 근거에 연결합니다.
- [상태 JSON](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/api/public/status.json)
- [실험 JSON](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/api/public/experiments.json)
- [지표 JSON](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/api/public/metrics.json)
- [기능 함수·로그 패킷](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/downloads/admission-review.txt)
- [애니메이션 함수·로그 패킷](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/downloads/animation-review.txt)

## 8. 잔여 문제
B8FB의8→64 실제 이동, CPU와 표시의 시간적 대응, 별도 반복 성능 기여도 실험이 남아 있습니다. 영상/GPU/compiled clip phase는 텍스트만으로 검증할 수 없습니다.

## 클라이언트 안내
인증/JavaScript 없이 GET할 수 있도록 만들었습니다. 라이브러리 기본 User-Agent에서 Cloudflare1010이 재현됐으므로 정체를 밝히는 User-Agent(예: DayZNPC-PublicReader/1.0)를 권장합니다. 실제 GPTBot의 접근 성공을 뜻하지 않습니다. 모든 외부 AI 도구의 접근은 아직 보증하지 않습니다.
