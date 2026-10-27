# DayZ NPC — AI 공개 연구 자료

기존 [사람용 대시보드](https://dayz-npc-observatory.sappyvole8.chatgpt.site/)를 유지하고, 다른 AI가 로그인 없이 읽는 JSON·Markdown과 선정된 함수/로그 발췌를 제공합니다.

## AI에게 전달할 URL

- [단일 검토 문서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/AI_REVIEW.md)
- [최신 실험 요약](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/latest.md)
- [상태 JSON](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/api/public/status.json)
- [실험 JSON](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/api/public/experiments.json)
- [지표 JSON](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/api/public/metrics.json)
- [공개 검증 기록](verification/latest.json)

원본 자료 시각: 2026-10-27T17:36:00.756451Z
snapshotId: `snapshot-20261027T185443179320Z-acd7bf8c`
staleAfter: 2026-10-28T17:36:00.756451Z

현재 UTC가 staleAfter 이후면 오래된 자료로 판단하세요. generatedAt/exportedAt은 새 게임 실험 수행 시각이 아닙니다. 수치마다 실행 ID·빌드·인원·조건·반복 횟수를 제공합니다. 진단 성공/실험 성공/최종 수용은 별개입니다.

## 근거 읽기

- [기능 함수·대표 로그](downloads/admission-review.txt)
- [애니메이션 함수·대표 로그](downloads/animation-review.txt)
- [JSON Schema](api/public/schema.json)
- [공개 파일 SHA-256 목록](manifest.json)

JSON과 원문 근거 파일은 원래 대시보드 공개 스냅샷의 바이트를 보존합니다. JSON의 `/downloads/...` 같은 canonical URI는 이 미러에서 `https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main` 뒤에 붙여 해석하세요. 원래 chatgpt.site 절대 링크가 읽히지 않으면 이 저장소의 상대 링크를 사용하세요.

전체 원시 로그·영상이 아닌 선정된 공개 자료입니다. 영상 품질이나 GPU/Present 시각을 텍스트만으로 검증했다고 주장하지 않습니다. 원본 설치 자산, 비공개 원시 기록, 게임 코드 전체, 자격증명, 로컬 경로·프로세스 정보는 포함하지 않습니다.

## 게시 및 검증

이 저장소는 외부 방문자에게 읽기 전용 공개 자료를 제공합니다. 실제 HTTP/스키마/해시 검증과 외부 AI 도구의 읽기 결과는 verification/latest.json에 별도 기록합니다. 일반 HTTP200을 모든 ChatGPT 세션의 읽기 성공으로 간주하지 않습니다. manifest의 상태는 내보내기 당시 상태입니다.
